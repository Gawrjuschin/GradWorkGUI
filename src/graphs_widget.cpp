#include "graphs_widget.h"
#include "graphs_data.h"
#include "graphs_switch.h"
#include "graphs_view.h"

#include <QElapsedTimer>
#include <QGraphicsLayout>
#include <QLegendMarker>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QValueAxis>
#include <QtAlgorithms>
#include <QtMath>

constexpr int TICK_COUNT = 10;
constexpr int LABEL_FONTSIZE = 12;
constexpr int TITLE_FONTSIZE = 18;

Graphs_Widget::Graphs_Widget(Graphs_Data* gdata,
                             QWidget *parent)
  : QWidget(parent)
  , p_chart_switch(new Graphs_Switch(this))
  , p_chart_view(new Graphs_View(nullptr, this))
  , p_vector_charts{}
  , p_points_data(gdata)
{
  auto* main_lo = new QVBoxLayout(this);
  auto* menu_lo = new QHBoxLayout;
  menu_lo->addWidget(p_chart_switch, 0);
  menu_lo->addStretch(1);
  main_lo->addLayout(menu_lo);
  main_lo->addWidget(p_chart_view,1);

  for(auto i = 0; i < p_vector_charts.size(); ++i)
    {
      adjust_graphs(i);

      connect(p_points_data->series(i), &QLineSeries::pointsReplaced,
              [=] { update_series(i); });
    }

  connect(p_chart_switch, &Graphs_Switch::signal_show,          this, &Graphs_Widget::slot_show);
  connect(p_chart_switch, &Graphs_Switch::signal_approximate,   this, &Graphs_Widget::slot_approximate);
  connect(p_chart_switch, &Graphs_Switch::signal_deapproximate, this, &Graphs_Widget::slot_deapproximate);
  connect(p_chart_switch, &Graphs_Switch::signal_save,          p_chart_view,&Graphs_View::slot_save);
  connect(p_chart_switch, &Graphs_Switch::signal_zoom_in,       p_chart_view, &Graphs_View::slot_zoom_in);
  connect(p_chart_switch, &Graphs_Switch::signal_zoom_out,      p_chart_view, &Graphs_View::slot_zoom_out);
  connect(p_chart_switch, &Graphs_Switch::signal_zoom_reset,    p_chart_view, &Graphs_View::slot_zoom_reset);
}

void Graphs_Widget::adjust_graphs(int i)
{
  QFont labels_font;
  labels_font.setPixelSize(LABEL_FONTSIZE);
  QPen axis_pen(Qt::black);
  axis_pen.setWidth(2);
  QFont title_font;
  title_font.setPixelSize(TITLE_FONTSIZE);
  QBrush title_brush(Qt::black);
  QBrush axis_brush(Qt::black);
  QPen grid_pen(Qt::gray);
  grid_pen.setWidth(1);

  p_vector_charts[i].reset(new QChart);

  p_vector_charts[i]->addSeries(p_points_data->series(i));
  p_vector_charts[i]->addSeries(p_points_data->series_apr(i));

  auto* x_axis = new QValueAxis;
  auto* y_axis = new QValueAxis;

  x_axis->setLabelsFont(labels_font);
  y_axis->setLabelsFont(labels_font);

  x_axis->setTickCount(TICK_COUNT);
  y_axis->setTickCount(TICK_COUNT);

  x_axis->setLinePen(axis_pen);
  y_axis->setLinePen(axis_pen);

  x_axis->setLabelFormat(QString::fromLatin1("%.2f "));
  y_axis->setLabelFormat(QString::fromLatin1("%.2f "));

  x_axis->setLabelsBrush(axis_brush);
  y_axis->setLabelsBrush(axis_brush);

  x_axis->setGridLinePen(grid_pen);
  y_axis->setGridLinePen(grid_pen);
  y_axis->setLabelsEditable(true);

  p_vector_charts[i]->addAxis(x_axis, Qt::AlignBottom);
  p_vector_charts[i]->addAxis(y_axis, Qt::AlignLeft);

  p_points_data->series(i)->attachAxis(x_axis);
  p_points_data->series(i)->attachAxis(y_axis);
  p_points_data->series_apr(i)->attachAxis(x_axis);
  p_points_data->series_apr(i)->attachAxis(y_axis);

  p_vector_charts[i]->setTitleFont(title_font);
  p_vector_charts[i]->setTitleBrush(title_brush);
  p_vector_charts[i]->setTitle(QString::fromLatin1(("Graph of dependency %1_%2(λ)"))
                               .arg( i >= 4 ? 'Z' : (i >= 2 ? 'U' : 'W') )
                               .arg((i%2) == 1 ? '1' : '0'));

  p_vector_charts[i]->setBackgroundVisible(false);

  p_vector_charts[i]->legend()->setAlignment(Qt::AlignBottom);
  p_vector_charts[i]->legend()->markers().at(0)->setLabel(tr("Experimental values"));
  p_vector_charts[i]->legend()->markers().at(1)->setLabel(tr("Exponential approximation"));

  p_vector_charts[i]->layout()->setContentsMargins(0, 0, 0, 0);
  p_vector_charts[i]->setMargins({0, 0, 0, 0});

  p_points_data->series(i)->show();
  p_points_data->series_apr(i)->hide();
}

Graphs_Widget::~Graphs_Widget() = default;

void Graphs_Widget::paintEvent(QPaintEvent *event)
{
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void Graphs_Widget::update_series(int index)
{
  auto points = p_points_data->series(index)->points();

  if(points.empty())
    {
      return;
    }

  QPointF x_range(points.front().x(),points.back().x());
  QPointF y_range = p_points_data->range(index);

  auto* x_axis = static_cast<QValueAxis*>(p_vector_charts[index]->axes().front());
  auto* y_axis = static_cast<QValueAxis*>(p_vector_charts[index]->axes().back());

  x_axis->setRange(x_range.x(), x_range.y());
  y_axis->setRange(y_range.x(), y_range.y());

  x_axis->setTickCount(TICK_COUNT);
  y_axis->setTickCount(TICK_COUNT);
  x_axis->applyNiceNumbers();
  y_axis->applyNiceNumbers();
}

void Graphs_Widget::slot_show(int index)
{
  if(p_chart_view->chart() != p_vector_charts[index].get())
    {
      p_chart_view->setChart(p_vector_charts[index].get());
    }
}

void Graphs_Widget::slot_approximate(int index)
{
  if(p_points_data->series_apr(index)->count() == 0)
    {
      p_points_data->approximate(index);
    }
  p_points_data->series_apr(index)->show();
  p_chart_view->repaint();
}

void Graphs_Widget::slot_deapproximate(int index)
{
  p_points_data->series_apr(index)->hide();
  p_chart_view->repaint();
}

void Graphs_Widget::slot_end()
{
  slot_show(0);
  emit signal_end();
}

void Graphs_Widget::slot_stop()
{
  p_chart_view->setDisabled(false);//Включаем меню
  p_chart_switch->setDisabled(false);//Включаем графы
}
