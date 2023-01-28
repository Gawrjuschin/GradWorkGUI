#include "results_widget.h"
#include "graphs_data.h"
#include "models.h"
#include "graphs_widget.h"
#include "loading_widget.h"
#include "table_widget.h"

#include <QLabel>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTableView>

constexpr int pix_size = 200;

Results_Widget::Results_Widget(std::shared_ptr<Table_Data> tdata,
                               std::shared_ptr<Graphs_Data> gdata,
                               QWidget *parent)
  : QWidget(parent)
  , p_widget_host(new QStackedWidget(this))
  , p_tabs_tables_graphs(new QTabWidget(this))
  , p_tables_evs_reqs(new Table_Widget(tdata.get(), this))
  , p_widget_graphs(new Graphs_Widget(gdata.get(), this))
  , p_widget_loading(new Loading_Widget(pix_size, this))
{

  auto* main_lo = new QVBoxLayout(this);
  main_lo->addWidget(p_widget_host);

  p_widget_host->addWidget(new QLabel(tr("Waiting for input parameters"), this));
  p_widget_host->addWidget(p_widget_loading);
  p_widget_host->addWidget(new QLabel(tr("Waiting for resume"), this));
  p_widget_host->addWidget(p_tabs_tables_graphs);
  p_widget_host->setCurrentIndex(WAITING);

  p_tabs_tables_graphs->addTab(p_tables_evs_reqs, tr("Tables"));
  p_tabs_tables_graphs->addTab(p_widget_graphs, tr("Graphs"));

  connect(this, &Results_Widget::signal_ready, p_widget_graphs, &Graphs_Widget::slot_end);
  connect(p_widget_graphs, &Graphs_Widget::signal_end, [&] { p_widget_host->setCurrentIndex(READY); });

}

Results_Widget::~Results_Widget( ) = default;

void Results_Widget::paintEvent(QPaintEvent *event)
{
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void Results_Widget::slot_start()
{
  p_widget_loading->slot_start();
  p_widget_host->setCurrentIndex(PROCESSING);
}

void Results_Widget::slot_pause()
{
  p_widget_loading->slot_stop();
  p_widget_host->setCurrentIndex(PAUSED);
}

void Results_Widget::slot_resume()
{
  p_widget_loading->slot_start();
  p_widget_host->setCurrentIndex(PROCESSING);
}

void Results_Widget::slot_stop()
{
  p_widget_loading->slot_stop();
  p_widget_host->setCurrentIndex(WAITING);
}

void Results_Widget::slot_done()
{
  emit signal_ready();
}
