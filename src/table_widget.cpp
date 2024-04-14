#include "table_widget.h"
#include "models.h"
#include "table_data.h"
#include <QHeaderView>
#include <QLabel>
#include <QPainter>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QVBoxLayout>

TableWidget::TableWidget(TableData& tdata, QWidget* parent)
    : QWidget(parent), r_tdata(tdata), p_tab_reqs(new QTableView),
      p_tab_evs(new QTableView), p_model_reqs(new RequestModel),
      p_model_evs(new EventModel) {
  auto* main_lo = new QVBoxLayout(this);

  main_lo->addWidget(new QLabel(tr("Requests:")));
  main_lo->addWidget(p_tab_reqs);
  main_lo->addWidget(new QLabel(tr("Events:")));
  main_lo->addWidget(p_tab_evs);

  auto* reqs_proxy = new QSortFilterProxyModel;
  reqs_proxy->setSourceModel(p_model_reqs);
  auto* evs_proxy = new QSortFilterProxyModel;
  evs_proxy->setSourceModel(p_model_evs);

  p_tab_reqs->setModel(reqs_proxy);
  p_tab_evs->setModel(evs_proxy);

  p_tab_reqs->setContentsMargins({0, 0, 0, 0});
  p_tab_evs->setContentsMargins({0, 0, 0, 0});

  p_tab_reqs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  p_tab_evs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  p_tab_reqs->setSortingEnabled(true);
  p_tab_evs->setSortingEnabled(true);

  p_tab_reqs->verticalHeader()->hide();
  p_tab_evs->verticalHeader()->hide();

  connect(p_model_reqs, &RequestModel::sigUpdate,
          [reqs_proxy]() { reqs_proxy->sort(0, Qt::AscendingOrder); });
  connect(p_model_evs, &EventModel::sigUpdate,
          [evs_proxy]() { evs_proxy->sort(1, Qt::AscendingOrder); });
}

TableWidget::~TableWidget() = default;

void TableWidget::onDataReady() {
  p_model_reqs->replace(r_tdata.requests);
  p_model_evs->replace(r_tdata.events);
}

// void Table_Widget::paintEvent(QPaintEvent* event)
// {
//   QStyleOption opt;
//   opt.initFrom(this);
//   QPainter p(this);
//   style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//   QWidget::paintEvent(event);
// }
