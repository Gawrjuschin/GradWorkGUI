#ifndef TABLE_DATA_H
#define TABLE_DATA_H

#include <QVector>

#include <QueSys/event.h>
#include <QueSys/request.h>

struct TableData
{
  static constexpr std::size_t kEventsNumber = 1000;
  static constexpr std::size_t kRequestsNumber = kEventsNumber / 2;

  QVector<Request> requests{};
  QVector<Event> events{};
};

#endif // TABLE_DATA_H
