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

namespace detail {

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

static inline QChart* MakeChart(int index) {
  auto* chart = new QChart;

  auto* exp_series = new QLineSeries;
  chart->addSeries(exp_series);

  auto* apr_series = new QLineSeries;
  chart->addSeries(apr_series);

  auto [x_axis, y_axis] = MakeAxis();
  chart->addAxis(x_axis, Qt::AlignBottom);
  chart->addAxis(y_axis, Qt::AlignLeft);

  exp_series->attachAxis(x_axis);
  exp_series->attachAxis(y_axis);

  apr_series->attachAxis(x_axis);
  apr_series->attachAxis(y_axis);

  QFont title_font;
  QBrush title_brush(Qt::black);
  title_font.setPixelSize(TITLE_FONTSIZE);
  chart->setTitleFont(title_font);
  chart->setTitleBrush(title_brush);
  chart->setTitle(QString((QObject::tr("Graph of dependency %1_%2(λ)")))
                      .arg(index >= 4 ? 'Z' : (index >= 2 ? 'U' : 'W'))
                      .arg((index % 2) == 1 ? '1' : '0'));

  chart->setBackgroundVisible(false);

  chart->legend()->setAlignment(Qt::AlignBottom);
  chart->legend()->markers().at(0)->setLabel(
      QObject::tr("Experimental values"));
  chart->legend()->markers().at(1)->setLabel(
      QObject::tr("Exponential approximation"));

  chart->layout()->setContentsMargins(0, 0, 0, 0);
  chart->setMargins({0, 0, 0, 0});

  exp_series->show();
  apr_series->hide();

  return chart;
}

} // namespace detail

GraphsWidget::GraphsWidget(const PointsData& points_data, QWidget* parent)
    : QWidget(parent), r_points_data(points_data),
      p_chart_switch(new GraphsSwitch(this)),
      p_chart_view(new GraphsView(nullptr, this)), charts_array{} {
  auto* main_lo = new QVBoxLayout(this);
  main_lo->addWidget(p_chart_switch);
  main_lo->addWidget(p_chart_view, 1);

  for (std::size_t i{}; i < std::size(charts_array); ++i) {
    charts_array[i] = detail::MakeChart(i);

    // connect(p_points_data->series(i), &QLineSeries::pointsReplaced, [=] {
    // update_series(i); });
  }

  connect(p_chart_switch, &GraphsSwitch::sigShow, this, &GraphsWidget::onShow);
  connect(p_chart_switch, &GraphsSwitch::sigApproximate, this,
          &GraphsWidget::onApproximate);
  connect(p_chart_switch, &GraphsSwitch::sigDeapproximate, this,
          &GraphsWidget::onDeapproximate);
  connect(p_chart_switch, &GraphsSwitch::sigSave, p_chart_view,
          &GraphsView::onSave);
  connect(p_chart_switch, &GraphsSwitch::sigZoomIn, p_chart_view,
          &GraphsView::onZoomIn);
  connect(p_chart_switch, &GraphsSwitch::sigZoomOut, p_chart_view,
          &GraphsView::onZoomOut);
  connect(p_chart_switch, &GraphsSwitch::sigZoomReset, p_chart_view,
          &GraphsView::onZoomReset);
}

GraphsWidget::~GraphsWidget() {
  for (auto* chart : charts_array) {
    if (chart != p_chart_view->chart())
      delete chart;
  }
}

// void GraphsWidget::paintEvent(QPaintEvent* event) {
//   QStyleOption opt;
//   opt.initFrom(this);
//   QPainter p(this);
//   style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//   QWidget::paintEvent(event);
// }

void GraphsWidget::updateSeries(int index) {
  const auto [y_min, y_max] = r_points_data.RangeY(index);

  auto* y_axis = static_cast<QValueAxis*>(charts_array[index]->axes().back());

  expSeries(index)->replace(r_points_data.pointsExperimental(index));
  aprSeries(index)->replace(r_points_data.pointsAproximation(index));

  y_axis->setRange(y_min, y_max);

  y_axis->setTickCount(TICK_COUNT);
  y_axis->applyNiceNumbers();

  aprSeries(index)->hide();
}

void GraphsWidget::onShow(int index) {
  if (p_chart_view->chart() != charts_array[index]) {
    p_chart_view->setChart(charts_array[index]);
  }
}

void GraphsWidget::onApproximate(int index) {
  aprSeries(index)->show();
  p_chart_view->repaint();
}

void GraphsWidget::onDeapproximate(int index) {
  aprSeries(index)->hide();
  p_chart_view->repaint();
}

void GraphsWidget::onPointsReady() {
  for (int index{}; index < PointsData::kGraphsCount; ++index) {
    updateSeries(index);
  }

  onShow(0);
  p_chart_switch->reset();

  emit sigEnd();
}
