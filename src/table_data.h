#ifndef TABLE_DATA_H
#define TABLE_DATA_H

#include <QVector>

#include <QueSys/event.h>
#include <QueSys/request.h>

struct TableData
{
  QVector<Request> requests{};
  QVector<Event> events{};
};

#endif // TABLE_DATA_H
