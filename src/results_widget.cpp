#include "results_widget.h"
#include "graphs_widget.h"
#include "loading_widget.h"
#include "points_data.h"
#include "table_widget.h"

#include <QLabel>
#include <QPainter>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>

namespace simulation_status {
constexpr auto kWaiting = 0;
constexpr auto kProcessing = 1;
constexpr auto kPaused = 2;
constexpr auto kReady = 3;
} // namespace simulation_status

constexpr int kPixSize = 200;

ResultsWidget::ResultsWidget(TableData& tdata, const PointsData& points_data,
                             QWidget* parent)
    : QWidget(parent), p_widget_host(new QStackedWidget(this)),
      p_tabs_widget(new QTabWidget(this)),
      p_table_widget(new TableWidget(tdata, this)),
      p_graphs_widget(new GraphsWidget(points_data, this)),
      p_loading_widget(new LoadingWidget(kPixSize, this)) {
  auto* main_lo = new QVBoxLayout(this);
  main_lo->addWidget(p_widget_host);

  p_widget_host->addWidget(new QLabel(tr("Waiting for input parameters"), this));
  p_widget_host->addWidget(p_loading_widget);
  p_widget_host->addWidget(new QLabel(tr("Waiting for resume"), this));
  p_widget_host->addWidget(p_tabs_widget);
  p_widget_host->setCurrentIndex(simulation_status::kWaiting);

  p_tabs_widget->addTab(p_table_widget, tr("Tables"));
  p_tabs_widget->addTab(p_graphs_widget, tr("Graphs"));

  connect(this, &ResultsWidget::sigDataReady, p_graphs_widget,
          &GraphsWidget::onPointsReady);
  connect(this, &ResultsWidget::sigDataReady, p_table_widget,
          &TableWidget::onDataReady);
  connect(p_graphs_widget, &GraphsWidget::sigEnd,
          [p_widget_host = p_widget_host] {
            p_widget_host->setCurrentIndex(simulation_status::kReady);
          });
}

ResultsWidget::~ResultsWidget() = default;

// void ResultsWidget::paintEvent(QPaintEvent *event)
// {
//   QStyleOption opt;
//   opt.initFrom(this);
//   QPainter p(this);
//   style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//   QWidget::paintEvent(event);
// }

void ResultsWidget::onStart() {
  p_loading_widget->onStart();
  p_widget_host->setCurrentIndex(simulation_status::kProcessing);
}

void ResultsWidget::onPause() {
  p_loading_widget->onStop();
  p_widget_host->setCurrentIndex(simulation_status::kPaused);
}

void ResultsWidget::onResume() {
  p_loading_widget->onStart();
  p_widget_host->setCurrentIndex(simulation_status::kProcessing);
}

void ResultsWidget::onStop() {
  p_loading_widget->onStop();
  p_widget_host->setCurrentIndex(simulation_status::kWaiting);
}

void ResultsWidget::onDataReady() { emit sigDataReady(); }
