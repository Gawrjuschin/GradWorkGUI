#ifndef event_H
#define event_H

#include <cstdint>
#include <iosfwd>
#include <utility>

namespace queueing_system {
/**
 * @brief The Event class - структура события СМО.
 */
struct Event {
  /**
   * @brief number - номер события.
   */
  std::uint32_t number{};
  /**
   * @brief time - время события.
   */
  double time{};
  /**
   * @brief type - тип события.
   */
  std::uint32_t type{};
  /**
   * @brief system_status - состояние приборов СМО на момент события.
   */
  std::pair<int, int> system_status{};
  /**
   * @brief queue_status - состояние очереди СМО на момент события.
   */
  std::pair<int, int> queue_status{};
  /**
   * @brief request - номер заявки, породившей событие.
   */
  std::uint32_t request{};
  /**
   * @brief time_next - время до следующей заявки.
   */
  double time_next{};
};

/**
 * @brief operator << - перегрузка оператора вывода.
 * @param os
 * @param event
 * @return
 */
std::ostream& operator<<(std::ostream& os, const Event& event);
/**
 * @brief operator >> - перегрузка оператора ввода.
 * @param os
 * @param event
 * @return
 */
std::istream& operator>>(std::istream& os, Event& event);

} // namespace queueing_system

#endif // event_H
