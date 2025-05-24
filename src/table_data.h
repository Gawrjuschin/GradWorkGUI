#ifndef TABLE_DATA_H
#define TABLE_DATA_H

#include <QVector>

#include <QueSys/event.hpp>
#include <QueSys/request.hpp>

/**
 * @brief The TableData class - данные таблиц заявок и событий СМО.
 */
struct TableData
{
  /**
   * @brief kEventsNumber - число событий СМО в таблице.
   */
  static constexpr std::size_t kEventsNumber = 1000;
  /**
   * @brief kRequestsNumber - максимальное число заявок в таблице (теоретический
   * предел).
   */
  static constexpr std::size_t kRequestsNumber = kEventsNumber / 2;
  /**
   * @brief requests - вектор данных заявок.
   */
  QVector<queueing_system::Request> requests{};
  /**
   * @brief events - вектор данных событий.
   */
  QVector<queueing_system::Event> events{};
};

#endif // TABLE_DATA_H
