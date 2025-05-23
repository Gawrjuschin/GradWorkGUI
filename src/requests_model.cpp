#include "requests_model.h"

namespace RequestsModelCol {
enum { NUMBER = 0, TYPE, ARRIVE, Q_NUM, CH_NUM, START, SERVE, WAIT, END };
}

RequestModel::RequestModel(QObject* parent) : QAbstractTableModel(parent) {}

RequestModel::~RequestModel() = default;

int RequestModel::rowCount(const QModelIndex&) const { return m_data.count(); }

int RequestModel::columnCount(const QModelIndex&) const { return COL_COUNT; }

QVariant RequestModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    return {};
  }
  const queueing_system::Request& data = m_data[index.row()];
  switch (index.column()) {
  case RequestsModelCol::NUMBER:
    return qint32(data.number);
  case RequestsModelCol::TYPE:
    return static_cast<qint32>(data.type);
  case RequestsModelCol::ARRIVE:
    return data.arrive_time;
  case RequestsModelCol::Q_NUM:
    return data.queue_number;
  case RequestsModelCol::CH_NUM:
    return (data.ch_number == -1 ? QString(tr("N/A"))
                                 : QString(tr("%1")).arg(data.ch_number));
  case RequestsModelCol::START:
    return (data.start_time == -1 ? QString(tr("N/A"))
                                  : QString(tr("%1")).arg(data.start_time));
  case RequestsModelCol::SERVE:
    return data.serve_time;
  case RequestsModelCol::WAIT:
    return (data.wait_time == -1 ? QString(tr("N/A"))
                                 : QString(tr("%1")).arg(data.wait_time));
  case RequestsModelCol::END:
    return (data.serve_end == -1 ? QString(tr("N/A"))
                                 : QString(tr("%1")).arg(data.serve_end));
  default:
    return {};
  }
}

QVariant RequestModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const { // Шапка таблицы
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
    return QVariant();
  }
  switch (section) {
  case RequestsModelCol::NUMBER:
    return tr("#");
  case RequestsModelCol::TYPE:
    return tr("type");
  case RequestsModelCol::ARRIVE:
    return tr("arrive");
  case RequestsModelCol::Q_NUM:
    return tr("q_num");
  case RequestsModelCol::CH_NUM:
    return tr("ch_num");
  case RequestsModelCol::START:
    return tr("start");
  case RequestsModelCol::SERVE:
    return tr("serve");
  case RequestsModelCol::WAIT:
    return tr("wait");
  case RequestsModelCol::END:
    return tr("serve");
  default:
    return QVariant();
  }
}

void RequestModel::swap(QVector<queueing_system::Request>& vec) {
  // По Мейерсу
  using std::swap;
  beginResetModel();
  swap(m_data, vec);
  endResetModel();
  emit sigUpdate();
}

void RequestModel::clear() {
  beginResetModel();
  m_data.clear();
  endResetModel();
}
