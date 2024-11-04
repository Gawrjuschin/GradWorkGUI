#ifndef event_H
#define event_H

#include <iosfwd>
#include <utility>

struct Event {
  int number{};             //Номер события
  double time{};            //Время события
  int type{};               //Тип события
  std::pair<int, int> system_status{}; // Состояние приборов СМО
  std::pair<int, int> queue_status{}; // Состояние очереди СМО
  int request{};            //Номер заявки
  double time_next{};       //Время до следующей заявки
};

std::ostream& operator<<(std::ostream&, const Event&);
std::istream& operator>>(std::istream&, Event&);

#endif // event_H
