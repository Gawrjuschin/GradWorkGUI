#include "event.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& s, const Event& event)
{
  constexpr int precision = 5;
  s << std::setiosflags(std::ios::fixed);
  s << std::setprecision(precision);
  s << event.number << "\t" << event.time << "\t" << event.type << "\t" << '('
    << event.system_status.first << ", " << event.system_status.second << "; "
    << event.queue_status.first << ", " << event.queue_status.second << ')' << "\t" << event.request
    << "\t" << event.time_next << std::endl;
  return s;
}

std::istream& operator>>(std::istream& s, Event& event)
{
  int number{0};
  int type{0};
  int request{0};
  double time{0};
  double time_next{0};
  std::pair system_status{0, 0};
  std::pair queue_status{0, 0};
  char syntax{0};
  if (s >> std::skipws >> number >> time >> type >> syntax >> system_status.first >> syntax
      >> system_status.second >> syntax >> queue_status.first >> syntax >> queue_status.second
      >> syntax >> request >> time_next) {
    event = {number, time, type, system_status, queue_status, request, time_next};
  } else {
    s.clear(std::ios_base::failbit);
  }
  return s;
}
