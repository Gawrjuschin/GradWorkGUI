#include "events_model.h"

namespace EventsModelCol {
enum { EV_NUM = 0, TIME, TYPE, S_STATUS, Q_STATUS, REQ_NUM, NEXT };
}

EventModel::EventModel(QObject* parent) : QAbstractTableModel(parent) {}

EventModel::~EventModel() = default;

int EventModel::rowCount(const QModelIndex&) const { return m_data.count(); }

int EventModel::columnCount(const QModelIndex&) const { return COL_COUNT; }

QVariant EventModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    return {};
  }

  const queueing_system::Event& data = m_data[index.row()];
  switch (index.column()) {
  case EventsModelCol::EV_NUM:
    return data.number;
  case EventsModelCol::TIME:
    return data.time;
  case EventsModelCol::TYPE:
    return data.type;
  case EventsModelCol::S_STATUS:
    return QString(tr("(%1, %2)"))
        .arg(data.system_status.first)
        .arg(data.system_status.second);
  case EventsModelCol::Q_STATUS:
    return QString(tr("(%1, %2)"))
        .arg(data.queue_status.first)
        .arg(data.queue_status.second);
  case EventsModelCol::REQ_NUM:
    return data.request;
  case EventsModelCol::NEXT:
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
  case EventsModelCol::EV_NUM:
    return tr("#");
  case EventsModelCol::TIME:
    return tr("time");
  case EventsModelCol::TYPE:
    return tr("type");
  case EventsModelCol::S_STATUS:
    return tr("S State");
  case EventsModelCol::Q_STATUS:
    return tr("Q State");
  case EventsModelCol::REQ_NUM:
    return tr("R #");
  case EventsModelCol::NEXT:
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

void EventModel::clear() {
  beginResetModel();
  m_data.clear();
  endResetModel();
}
