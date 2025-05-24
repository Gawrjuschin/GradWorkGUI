#ifndef request_info_H
#define request_info_H

#include <cstdint>
#include <iosfwd>

namespace queueing_system {
enum class RequestType { kNone = 0, kFirst = 1, kSecond = 2 };

/**
 * @brief operator << - перегрузка оператора вывода.
 * @param os
 * @param type
 * @return
 */
std::ostream& operator<<(std::ostream& os, const RequestType& type);
/**
 * @brief operator >> - перегрузка оператора ввода.
 * @param is
 * @param type
 * @return
 */
std::istream& operator>>(std::istream& is, RequestType& type);

/**
 * @brief The Request class - структура заявки СМО.
 */
struct Request {
  /**
   * @brief number - номер заявки
   */
  std::uint32_t number{};
  /**
   * @brief type - тип заявки (None, 1 или 2)
   */
  RequestType type{RequestType::kNone};
  /**
   * @brief arrive_time - время прибытия
   */
  double arrive_time{};
  /**
   * @brief queue_number - номер в очереди
   */
  std::uint32_t queue_number{};
  /**
   * @brief ch_number - номер прибора, обслуживающего заявку
   */
  std::uint32_t ch_number{};
  /**
   * @brief start_time - время начала обслуживания
   */
  double start_time{};
  /**
   * @brief serve_time - время обслуживания
   */
  double serve_time{};
  /**
   * @brief wait_time - время ожидания в очереди
   */
  double wait_time{};
  /**
   * @brief serve_end - время окончания обслуживания
   */
  double serve_end{};
};

/**
 * @brief operator << - перегрузка оператора вывода.
 * @param os
 * @param request
 * @return
 */
std::ostream& operator<<(std::ostream& os, const Request& request);
/**
 * @brief operator >> - перегрузка оператора ввода.
 * @param is
 * @param request
 * @return
 */
std::istream& operator>>(std::istream& is, Request& request);

// Определяет значение по умолчанию
inline bool IsEmpty(const Request& req) noexcept {
  return req.type == RequestType::kNone;
}

} // namespace queueing_system

#endif // request_info_H
