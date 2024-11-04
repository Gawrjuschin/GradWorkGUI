#ifndef request_info_H
#define request_info_H

#include <iosfwd>

enum class RequestType { kNone = 0, kFirst = 1, kSecond = 2 };

std::ostream& operator<<(std::ostream& os, const RequestType& type);
std::istream& operator>>(std::istream& is, RequestType& type);

struct Request {
  int number{};                         // Номер заявки
  RequestType type{RequestType::kNone}; //Тип заявки (None, 1 или 2)
  double arrive_time{};                 //Время прибытия
  int queue_number{};                   //Номер в очереди
  int ch_number{};     //Номер прибора, обслуживающего заявку
  double start_time{}; //Время начала обслуживания
  double serve_time{}; //Время обслуживания
  double wait_time{};  //Время ожидания в очереди
  double serve_end{};  //Время окончания обслуживания
};

std::ostream& operator<<(std::ostream& os, const Request& request);
std::istream& operator>>(std::istream& is, Request& request);

// Определяет значение по умолчанию
inline bool IsEmpty(const Request& req) noexcept
{
  return req.type == RequestType::kNone;
}

#endif // request_info_H
