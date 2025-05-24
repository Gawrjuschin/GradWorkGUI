#include "events_model.h"

namespace EventsModelCol {
enum {
  kEventNumber = 0,
  kTime,
  kType,
  kSystemStatus,
  kQueueStatus,
  kRequestNumber,
  kNext
};
}

EventModel::EventModel(QObject* parent) : QAbstractTableModel(parent) {}

EventModel::~EventModel() = default;

int EventModel::rowCount(const QModelIndex&) const { return m_data.count(); }

int EventModel::columnCount(const QModelIndex&) const { return kColCount; }

QVariant EventModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    return {};
  }

  const queueing_system::Event& data = m_data[index.row()];
  switch (index.column()) {
  case EventsModelCol::kEventNumber:
    return data.number;
  case EventsModelCol::kTime:
    return data.time;
  case EventsModelCol::kType:
    return data.type;
  case EventsModelCol::kSystemStatus:
    return QString(tr("(%1, %2)"))
        .arg(data.system_status.first)
        .arg(data.system_status.second);
  case EventsModelCol::kQueueStatus:
    return QString(tr("(%1, %2)"))
        .arg(data.queue_status.first)
        .arg(data.queue_status.second);
  case EventsModelCol::kRequestNumber:
    return data.request;
  case EventsModelCol::kNext:
    return data.time_next;
  default:
    return {};
  }
}

QVariant EventModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
    return {};
  }

  switch (section) {
  case EventsModelCol::kEventNumber:
    return tr("#");
  case EventsModelCol::kTime:
    return tr("time");
  case EventsModelCol::kType:
    return tr("type");
  case EventsModelCol::kSystemStatus:
    return tr("S State");
  case EventsModelCol::kQueueStatus:
    return tr("Q State");
  case EventsModelCol::kRequestNumber:
    return tr("R #");
  case EventsModelCol::kNext:
    return tr("Next");
  default:
    return {};
  }
}

void EventModel::swap(QVector<queueing_system::Event>& vec) {
  beginResetModel();
  m_data.swap(vec);
  endResetModel();
  emit sigUpdate();
}

std::ostream& EventModel::asText(std::ostream& os) const {
  for (const auto& event : m_data) {
    os << event.number << '\t' << event.time << '\t' << event.type << "\t("
       << event.system_status.first << ',' << event.system_status.second
       << ")\t(" << event.queue_status.first << ',' << event.queue_status.second
       << ")\t" << event.request << '\t' << event.time_next << '\n';
  }
  return os;
}

void EventModel::clear() {
  beginResetModel();
  m_data.clear();
  endResetModel();
}
