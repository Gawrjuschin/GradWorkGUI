#ifndef event_H
#define event_H

#include <cstdint>
#include <iosfwd>
#include <utility>

namespace queueing_system {
/**
 * @brief The Event class - структура события СМО
 */
struct Event {
  std::uint32_t number{};              // Номер события
  double time{};                       // Время события
  std::uint32_t type{};                // Тип события
  std::pair<int, int> system_status{}; // Состояние приборов СМО
  std::pair<int, int> queue_status{};  // Состояние очереди СМО
  std::uint32_t request{};             // Номер заявки
  double time_next{};                  // Время до следующей заявки
};

std::ostream& operator<<(std::ostream& os, const Event& event);
std::istream& operator>>(std::istream& os, Event& event);

} // namespace queueing_system

#endif // event_H
