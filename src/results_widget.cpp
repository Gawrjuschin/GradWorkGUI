#include "results_widget.h"
#include "graphs_widget.h"
#include "loading_widget.h"
#include "points_data.h"
#include "table_widget.h"

#include <QLabel>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTableView>

constexpr int pix_size = 200;

Results_Widget::Results_Widget(TableData& tdata, const PointsData& points_data,
                               QWidget* parent)
    : QWidget(parent), p_widget_host(new QStackedWidget(this)),
      p_tabs_widget(new QTabWidget(this)),
      p_table_widget(new TableWidget(tdata, this)),
      p_graphs_widget(new Graphs_Widget(points_data, this)),
      p_loading_widget(new LoadingWidget(pix_size, this)) {
  auto* main_lo = new QVBoxLayout(this);
  main_lo->addWidget(p_widget_host);

  p_widget_host->addWidget(new QLabel(tr("Waiting for input parameters"), this));
  p_widget_host->addWidget(p_loading_widget);
  p_widget_host->addWidget(new QLabel(tr("Waiting for resume"), this));
  p_widget_host->addWidget(p_tabs_widget);
  p_widget_host->setCurrentIndex(WAITING);

  p_tabs_widget->addTab(p_table_widget, tr("Tables"));
  p_tabs_widget->addTab(p_graphs_widget, tr("Graphs"));

  connect(this, &Results_Widget::sigDataReady, p_graphs_widget,
          &Graphs_Widget::onPointsReady);
  connect(this, &Results_Widget::sigDataReady, p_table_widget,
          &TableWidget::onDataReady);
  connect(p_graphs_widget, &Graphs_Widget::sigEnd,
          [&] { p_widget_host->setCurrentIndex(READY); });
}

Results_Widget::~Results_Widget() = default;

void Results_Widget::paintEvent(QPaintEvent *event)
{
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void Results_Widget::onStart() {
  p_loading_widget->onStart();
  p_widget_host->setCurrentIndex(PROCESSING);
}

void Results_Widget::onPause() {
  p_loading_widget->onStop();
  p_widget_host->setCurrentIndex(PAUSED);
}

void Results_Widget::onResume() {
  p_loading_widget->onStart();
  p_widget_host->setCurrentIndex(PROCESSING);
}

void Results_Widget::onStop() {
  p_loading_widget->onStop();
  p_widget_host->setCurrentIndex(WAITING);
}

void Results_Widget::onDataReady() { emit sigDataReady(); }
