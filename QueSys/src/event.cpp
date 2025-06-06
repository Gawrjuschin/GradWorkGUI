#include "event.hpp"

#include <cstdint>
#include <iomanip>

namespace queueing_system {
std::ostream& operator<<(std::ostream& os, const Event& event) {
  constexpr int precision = 5;
  os << std::setiosflags(std::ios::fixed);
  os << std::setprecision(precision);
  os << event.number << "\t" << event.time << "\t" << event.type << "\t" << '('
     << event.system_status.first << ", " << event.system_status.second << "; "
     << event.queue_status.first << ", " << event.queue_status.second << ')'
     << "\t" << event.request << "\t" << event.time_next << std::endl;
  return os;
}

std::istream& operator>>(std::istream& os, Event& event) {
  std::uint32_t number{0};
  std::uint32_t type{0};
  std::uint32_t request{0};
  double time{0};
  double time_next{0};
  std::pair system_status{0, 0};
  std::pair queue_status{0, 0};
  char syntax{0};
  if (os >> std::skipws >> number >> time >> type >> syntax >>
      system_status.first >> syntax >> system_status.second >> syntax >>
      queue_status.first >> syntax >> queue_status.second >> syntax >>
      request >> time_next) {
    event = {number,       time,    type,     system_status,
             queue_status, request, time_next};
  } else {
    os.clear(std::ios_base::failbit);
  }
  return os;
}

} // namespace queueing_system
