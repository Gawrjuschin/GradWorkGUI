#include "graphs_widget.h"
#include "graphs_switch.h"
#include "graphs_view.h"
#include "points_data.h"

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

Graphs_Widget::Graphs_Widget(const PointsData& points_data, QWidget* parent)
    : QWidget(parent), r_points_data(points_data),
      p_chart_switch(new Graphs_Switch(this)),
      p_chart_view(new Graphs_View(nullptr, this)), charts_array{} {
  auto* main_lo = new QVBoxLayout(this);
  auto* menu_lo = new QHBoxLayout;
  menu_lo->addWidget(p_chart_switch, 0);
  menu_lo->addStretch(1);
  main_lo->addLayout(menu_lo);
  main_lo->addWidget(p_chart_view, 1);

  for (std::size_t i = 0; i < charts_array.size(); ++i) {
    adjust_charts(i);

    // connect(p_points_data->series(i), &QLineSeries::pointsReplaced, [=] {
    // update_series(i); });
  }

  connect(p_chart_switch, &Graphs_Switch::sigShow, this,
          &Graphs_Widget::onShow);
  connect(p_chart_switch, &Graphs_Switch::sigApproximate, this,
          &Graphs_Widget::onApproximate);
  connect(p_chart_switch, &Graphs_Switch::sigDeapproximate, this,
          &Graphs_Widget::onDeapproximate);
  connect(p_chart_switch, &Graphs_Switch::sigSave, p_chart_view,
          &Graphs_View::onSave);
  connect(p_chart_switch, &Graphs_Switch::sigZoomIn, p_chart_view,
          &Graphs_View::onZoomIn);
  connect(p_chart_switch, &Graphs_Switch::sigZoomOut, p_chart_view,
          &Graphs_View::onZoomOut);
  connect(p_chart_switch, &Graphs_Switch::sigZoomReset, p_chart_view,
          &Graphs_View::onZoomReset);
}

static inline std::pair<QValueAxis*, QValueAxis*> MakeAxis() {
  QFont labels_font;
  labels_font.setPixelSize(LABEL_FONTSIZE);
  QPen axis_pen(Qt::black);
  axis_pen.setWidth(2);
  QBrush axis_brush(Qt::black);
  QPen grid_pen(Qt::gray);
  grid_pen.setWidth(1);

  auto* x_axis = new QValueAxis;
  auto* y_axis = new QValueAxis;

  x_axis->setLabelsFont(labels_font);
  y_axis->setLabelsFont(labels_font);

  x_axis->setRange(PointsData::kMinLoad, PointsData::kMaxLoad);

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

  return {x_axis, y_axis};
}

void Graphs_Widget::adjust_charts(int i) {
  charts_array[i] = std::make_unique<QChart>();
  exp_series_array[i] = new QLineSeries;
  apr_series_array[i] = new QLineSeries;
  charts_array[i]->addSeries(exp_series_array[i]);
  charts_array[i]->addSeries(apr_series_array[i]);

  auto [x_axis, y_axis] = MakeAxis();

  charts_array[i]->addAxis(x_axis, Qt::AlignBottom);
  charts_array[i]->addAxis(y_axis, Qt::AlignLeft);

  exp_series_array[i]->attachAxis(x_axis);
  exp_series_array[i]->attachAxis(y_axis);
  apr_series_array[i]->attachAxis(x_axis);
  apr_series_array[i]->attachAxis(y_axis);

  QFont title_font;
  QBrush title_brush(Qt::black);
  title_font.setPixelSize(TITLE_FONTSIZE);
  charts_array[i]->setTitleFont(title_font);
  charts_array[i]->setTitleBrush(title_brush);
  charts_array[i]->setTitle(QString((tr("Graph of dependency %1_%2(λ)")))
                                .arg(i >= 4 ? 'Z' : (i >= 2 ? 'U' : 'W'))
                                .arg((i % 2) == 1 ? '1' : '0'));

  charts_array[i]->setBackgroundVisible(false);

  charts_array[i]->legend()->setAlignment(Qt::AlignBottom);
  charts_array[i]->legend()->markers().at(0)->setLabel(
      tr("Experimental values"));
  charts_array[i]->legend()->markers().at(1)->setLabel(
      tr("Exponential approximation"));

  charts_array[i]->layout()->setContentsMargins(0, 0, 0, 0);
  charts_array[i]->setMargins({0, 0, 0, 0});

  exp_series_array[i]->show();
  apr_series_array[i]->hide();
}

Graphs_Widget::~Graphs_Widget() = default;

void Graphs_Widget::paintEvent(QPaintEvent* event) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void Graphs_Widget::update_series(int index) {
  auto points = exp_series_array[index]->points();

  const auto [y_min, y_max] = r_points_data.RangeY(index);

  auto* y_axis = static_cast<QValueAxis*>(charts_array[index]->axes().back());

  exp_series_array[index]->replace(r_points_data.pointsExperimental(index));
  apr_series_array[index]->replace(r_points_data.pointsAproximation(index));

  y_axis->setRange(y_min, y_max);

  y_axis->setTickCount(TICK_COUNT);
  y_axis->applyNiceNumbers();
}

void Graphs_Widget::onShow(int index) {
  if (p_chart_view->chart() != charts_array[index].get()) {
    p_chart_view->setChart(charts_array[index].get());
  }
}

void Graphs_Widget::onApproximate(int index) {
  apr_series_array[index]->show();
  p_chart_view->repaint();
}

void Graphs_Widget::onDeapproximate(int index) {
  apr_series_array[index]->hide();
  p_chart_view->repaint();
}

void Graphs_Widget::onPointsReady() {
  for (int graph_number{}; graph_number < PointsData::kGraphsCount;
       ++graph_number) {
    update_series(graph_number);
  }
  onShow(0);
  emit sigEnd();
}

void Graphs_Widget::onStop() {
  p_chart_view->setDisabled(false);   // Включаем меню
  p_chart_switch->setDisabled(false); // Включаем графы
}
