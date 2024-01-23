#include "models.h"

#include <QAbstractTableModel>
#include <QAction>
#include <QHeaderView>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~ Модель заявок ~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Request_Model::Request_Model(QObject* parent)
    : QAbstractTableModel(parent)
{}

Request_Model::~Request_Model() = default;

int Request_Model::rowCount(const QModelIndex&) const
{
  return m_data.count();
}

int Request_Model::columnCount(const QModelIndex&) const
{
  return COL_COUNT;
}

QVariant Request_Model::data(const QModelIndex& index, int role) const
{
  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    return QVariant();
  }
  const Request& data = m_data[index.row()];
  switch (index.column()) {
    case NUMBER:
      return qint32(data.number);
    case TYPE:
      return static_cast<qint32>(data.type);
    case ARRIVE:
      return data.arrive_time;
    case Q_NUM:
      return data.queue_number;
    case CH_NUM:
      return (data.ch_number == -1 ? QString(tr("N/A")) : QString(tr("%1")).arg(data.ch_number));
    case START:
      return (data.start_time == -1 ? QString(tr("N/A")) : QString(tr("%1")).arg(data.start_time));
    case SERVE:
      return data.serve_time;
    case WAIT:
      return (data.wait_time == -1 ? QString(tr("N/A")) : QString(tr("%1")).arg(data.wait_time));
    case END:
      return (data.serve_end == -1 ? QString(tr("N/A")) : QString(tr("%1")).arg(data.serve_end));
    default:
      return QVariant();
  }
}

QVariant Request_Model::headerData(int section, Qt::Orientation orientation, int role) const
{ //Шапка таблицы
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
    return QVariant();
  }
  switch (section) {
    case NUMBER:
      return tr("#");
    case TYPE:
      return tr("type");
    case ARRIVE:
      return tr("arrive");
    case Q_NUM:
      return tr("q_num");
    case CH_NUM:
      return tr("ch_num");
    case START:
      return tr("start");
    case SERVE:
      return tr("serve");
    case WAIT:
      return tr("wait");
    case END:
      return tr("serve");
    default:
      return QVariant();
  }
}

void Request_Model::append(const Request& data)
{ //When reimplementing insertRows() in a subclass, you must
  //call this function before inserting data into the model's
  //underlying data store
  beginInsertRows({}, m_data.count(), m_data.count());
  m_data.append(data);
  endInsertRows();
  emit signal_update();
}

void Request_Model::append(QVector<Request> vec)
{
  beginResetModel();
  m_data = vec;
  endResetModel();
  emit signal_update();
}

void Request_Model::clear()
{
  beginResetModel();
  m_data.clear();
  endResetModel();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~ Модель событий ~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Event_Model::Event_Model(QObject* parent)
    : QAbstractTableModel(parent)
{}

Event_Model::~Event_Model() = default;

int Event_Model::rowCount(const QModelIndex&) const
{
  return m_data.count();
}

int Event_Model::columnCount(const QModelIndex&) const
{
  return COL_COUNT;
}

QVariant Event_Model::data(const QModelIndex& index, int role) const
{
  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    return QVariant();
  }
  const Event& data = m_data[index.row()];
  switch (index.column()) {
    case EV_NUM:
      return data.number;
    case TIME:
      return data.time;
    case TYPE:
      return data.type;
    case S_STATUS:
      return QString(tr("(%1, %2)")).arg(data.system_status.first).arg(data.system_status.second);
    case Q_STATUS:
      return QString(tr("(%1, %2)")).arg(data.queue_status.first).arg(data.queue_status.second);
    case REQ_NUM:
      return data.request;
    case NEXT:
      return data.time_next;
    default:
      return QVariant();
  }
}

QVariant Event_Model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
    return QVariant();
  }
  switch (section) {
    case EV_NUM:
      return tr("#");
    case TIME:
      return tr("time");
    case TYPE:
      return tr("type");
    case S_STATUS:
      return tr("S State");
    case Q_STATUS:
      return tr("Q State");
    case REQ_NUM:
      return tr("R #");
    case NEXT:
      return tr("Next");
    default:
      return QVariant();
  }
}

void Event_Model::append(const Event& data)
{
  beginInsertRows({}, m_data.count(), m_data.count());
  m_data.append(data);
  endInsertRows();
  emit signal_update();
}

void Event_Model::append(QVector<Event> vec)
{
  beginResetModel();
  m_data = vec;
  endResetModel();
  emit signal_update();
}

void Event_Model::clear()
{
  beginResetModel();
  m_data.clear();
  endResetModel();
}
