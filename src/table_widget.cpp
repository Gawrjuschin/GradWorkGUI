#include "table_widget.h"
#include "models.h"
#include "table_data.h"

#include <QHeaderView>
#include <QLabel>
#include <QPainter>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QVBoxLayout>

Table_Widget::Table_Widget(Table_Data* tdata, QWidget* parent)
    : QWidget(parent)
    , p_tab_reqs(new QTableView)
    , p_tab_evs(new QTableView)
{
  auto* main_lo = new QVBoxLayout(this);

  main_lo->addWidget(new QLabel(tr("Requests:")));
  main_lo->addWidget(p_tab_reqs);
  main_lo->addWidget(new QLabel(tr("Events:")));
  main_lo->addWidget(p_tab_evs);

  auto* rproxy = new QSortFilterProxyModel;
  rproxy->setSourceModel(tdata->request_model());
  auto* eproxy = new QSortFilterProxyModel;
  eproxy->setSourceModel(tdata->event_model());

  p_tab_reqs->setModel(rproxy);
  p_tab_evs->setModel(eproxy);

  p_tab_reqs->setContentsMargins({0, 0, 0, 0});
  p_tab_evs->setContentsMargins({0, 0, 0, 0});

  p_tab_reqs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  p_tab_evs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  p_tab_reqs->setSortingEnabled(true);
  p_tab_evs->setSortingEnabled(true);

  p_tab_reqs->verticalHeader()->hide();
  p_tab_evs->verticalHeader()->hide();

  connect(tdata->request_model(), &Request_Model::signal_update, [&]() {
    p_tab_reqs->model()->sort(0, Qt::AscendingOrder);
  });
  connect(tdata->event_model(), &Event_Model::signal_update, [&]() {
    p_tab_evs->model()->sort(1, Qt::AscendingOrder);
  });
}

Table_Widget::~Table_Widget() = default;

// void Table_Widget::paintEvent(QPaintEvent* event)
// {
//   QStyleOption opt;
//   opt.initFrom(this);
//   QPainter p(this);
//   style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//   QWidget::paintEvent(event);
// }
