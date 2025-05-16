#ifndef request_info_H
#define request_info_H

#include <cstdint>
#include <iosfwd>

namespace queueing_system {
enum class RequestType { kNone = 0, kFirst = 1, kSecond = 2 };

std::ostream& operator<<(std::ostream& os, const RequestType& type);
std::istream& operator>>(std::istream& is, RequestType& type);

/**
 * @brief The Request class - структура заявки СМО
 */
struct Request {
  std::uint32_t number{};               // Номер заявки
  RequestType type{RequestType::kNone}; // Тип заявки (None, 1 или 2)
  double arrive_time{};                 // Время прибытия
  std::uint32_t queue_number{};         // Номер в очереди
  std::uint32_t ch_number{};            // Номер прибора, обслуживающего заявку
  double start_time{};                  // Время начала обслуживания
  double serve_time{};                  // Время обслуживания
  double wait_time{};                   // Время ожидания в очереди
  double serve_end{};                   // Время окончания обслуживания
};

std::ostream& operator<<(std::ostream& os, const Request& request);
std::istream& operator>>(std::istream& is, Request& request);

// Определяет значение по умолчанию
inline bool IsEmpty(const Request& req) noexcept {
  return req.type == RequestType::kNone;
}

} // namespace queueing_system

#endif // request_info_H
