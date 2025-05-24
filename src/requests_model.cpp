#include "requests_model.h"

namespace RequestsModelCol {
enum {
  kNumber = 0,
  kType,
  kArrive,
  kQueueNumber,
  kChannelNumber,
  kStart,
  kServe,
  kWait,
  kEnd
};
}

RequestModel::RequestModel(QObject* parent) : QAbstractTableModel(parent) {}

RequestModel::~RequestModel() = default;

int RequestModel::rowCount(const QModelIndex&) const { return m_data.count(); }

int RequestModel::columnCount(const QModelIndex&) const { return kColCount; }

QVariant RequestModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    return {};
  }
  const queueing_system::Request& data = m_data[index.row()];
  switch (index.column()) {
  case RequestsModelCol::kNumber:
    return qint32(data.number);
  case RequestsModelCol::kType:
    return static_cast<qint32>(data.type);
  case RequestsModelCol::kArrive:
    return data.arrive_time;
  case RequestsModelCol::kQueueNumber:
    return data.queue_number;
  case RequestsModelCol::kChannelNumber:
    return ((data.start_time == 0) ? QString(tr("N/A"))
                                   : QString(tr("%1")).arg(data.ch_number));
  case RequestsModelCol::kStart:
    return ((data.start_time == 0) ? QString(tr("N/A"))
                                   : QString(tr("%1")).arg(data.start_time));
  case RequestsModelCol::kServe:
    return data.serve_time;
  case RequestsModelCol::kWait:
    return QString(tr("%1")).arg(data.wait_time);
  case RequestsModelCol::kEnd:
    return ((data.serve_end == 0) ? QString(tr("N/A"))
                                  : QString(tr("%1")).arg(data.serve_end));
  default:
    return {};
  }
}

QVariant RequestModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
    return QVariant();
  }
  switch (section) {
  case RequestsModelCol::kNumber:
    return tr("#");
  case RequestsModelCol::kType:
    return tr("type");
  case RequestsModelCol::kArrive:
    return tr("arrive");
  case RequestsModelCol::kQueueNumber:
    return tr("q_num");
  case RequestsModelCol::kChannelNumber:
    return tr("ch_num");
  case RequestsModelCol::kStart:
    return tr("start");
  case RequestsModelCol::kServe:
    return tr("serve");
  case RequestsModelCol::kWait:
    return tr("wait");
  case RequestsModelCol::kEnd:
    return tr("end");
  default:
    return QVariant();
  }
}

void RequestModel::swap(QVector<queueing_system::Request>& vec) {
  beginResetModel();
  m_data.swap(vec);
  endResetModel();
  emit sigUpdate();
}

std::ostream& RequestModel::asText(std::ostream& os) const {

  for (const auto& request : m_data) {
    os << request.number << '\t' << static_cast<int>(request.type) << '\t'
       << request.arrive_time << '\t' << request.queue_number << '\t';

    if (request.start_time == 0) {
      os << "N/A\tN/A\t";
    } else {
      os << request.ch_number << '\t' << request.start_time << '\t';
    }

    os << request.serve_time << '\t' << request.wait_time << '\t';

    if (request.serve_end == 0) {
      os << "N/A";
    } else {
      os << request.serve_end;
    }
    os << '\n';
  }
  return os;
}

void RequestModel::clear() {
  beginResetModel();
  m_data.clear();
  endResetModel();
}
