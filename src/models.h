#ifndef MODELS_H
#define MODELS_H

#include <QAbstractTableModel>
#include <QueSys/queueing_system.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~ Модель заявок ~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class RequestModel : public QAbstractTableModel {
  Q_OBJECT

public:
  enum { COL_COUNT = 9 };

  RequestModel(QObject* parent = 0);

  virtual ~RequestModel();

  virtual int rowCount(const QModelIndex&) const override;

  virtual int columnCount(const QModelIndex&) const override;

  virtual QVariant data(const QModelIndex& index, int role) const override;

  virtual QVariant headerData(int section, Qt::Orientation orientation,
                              int role) const override;

  // Обменивается векторами с TableData
  void replace(QVector<Request>& vec);

  void append(const Request& data);

  void append(QVector<Request> vec);

  void clear();

signals:
  void signal_update();

private:
  QVector<Request> m_data{};

  enum { NUMBER = 0, TYPE, ARRIVE, Q_NUM, CH_NUM, START, SERVE, WAIT, END };
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~ Модель событий ~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class EventModel : public QAbstractTableModel {
  Q_OBJECT

public:
  enum { COL_COUNT = 7 };

  EventModel(QObject* parent = 0);

  virtual ~EventModel();

  virtual int rowCount(const QModelIndex&) const override;

  virtual int columnCount(const QModelIndex&) const override;

  virtual QVariant data(const QModelIndex& index, int role) const override;

  virtual QVariant headerData(int section, Qt::Orientation orientation,
                              int role) const override;

  // Обменивается векторами с TableData
  void replace(QVector<Event>& vec);

  void append(const Event& data);

  void append(QVector<Event> vec);

  void clear();

signals:
  void signal_update();

private:
  QVector<Event> m_data;

  enum { EV_NUM = 0, TIME, TYPE, S_STATUS, Q_STATUS, REQ_NUM, NEXT };
};

#endif // MODELS_H
