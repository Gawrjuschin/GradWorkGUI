#ifndef TABLE_DATA_H
#define TABLE_DATA_H

#include <QVector>

#include <QueSys/event.hpp>
#include <QueSys/request.hpp>

struct TableData
{
  static constexpr std::size_t kEventsNumber = 1000;
  static constexpr std::size_t kRequestsNumber = kEventsNumber / 2;

  QVector<queueing_system::Request> requests{};
  QVector<queueing_system::Event> events{};
};

#endif // TABLE_DATA_H
