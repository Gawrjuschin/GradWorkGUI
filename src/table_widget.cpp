#include "table_widget.h"
#include "events_model.h"
#include "requests_model.h"
#include "table_data.h"

#include <QApplication>
#include <QClipboard>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <sstream>

namespace detail {
static inline auto MakeClipboardCb(auto* model) {
  return [model] {
    std::stringstream sstream;
    model->asText(sstream);

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(QString::fromStdString(sstream.str()));
  };
}
} // namespace detail

TableWidget::TableWidget(TableData& tdata, QWidget* parent)
    : QWidget(parent), r_tdata(tdata), p_tab_reqs(new QTableView),
      p_tab_evs(new QTableView), p_requests_model(new RequestModel),
      p_events_model(new EventModel) {
  auto* main_lo = new QGridLayout(this);

  auto* requests_copy_btn = new QPushButton(tr("Copy"));
  main_lo->addWidget(new QLabel(tr("Requests:")), 0, 0, 1, 1, Qt::AlignLeft);
  main_lo->addWidget(requests_copy_btn, 0, 1, 1, 1, Qt::AlignRight);
  main_lo->addWidget(p_tab_reqs, 1, 0, 1, 2);

  auto* events_copy_btn = new QPushButton(tr("Copy"));
  main_lo->addWidget(new QLabel(tr("Events:")), 2, 0, 1, 1, Qt::AlignLeft);
  main_lo->addWidget(events_copy_btn, 2, 1, 1, 1, Qt::AlignRight);
  main_lo->addWidget(p_tab_evs, 3, 0, 1, 2);

  auto* reqs_proxy = new QSortFilterProxyModel;
  reqs_proxy->setSourceModel(p_requests_model);
  auto* evs_proxy = new QSortFilterProxyModel;
  evs_proxy->setSourceModel(p_events_model);

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

  connect(p_requests_model, &RequestModel::sigUpdate,
          [reqs_proxy]() { reqs_proxy->sort(0, Qt::AscendingOrder); });
  connect(p_events_model, &EventModel::sigUpdate,
          [evs_proxy]() { evs_proxy->sort(1, Qt::AscendingOrder); });

  connect(requests_copy_btn, &QPushButton::clicked,
          detail::MakeClipboardCb(p_requests_model));

  connect(events_copy_btn, &QPushButton::clicked,
          detail::MakeClipboardCb(p_events_model));
}

TableWidget::~TableWidget() = default;

void TableWidget::onDataReady() {
  p_requests_model->swap(r_tdata.requests);
  p_events_model->swap(r_tdata.events);
}
