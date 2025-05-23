#ifndef req_queue_H
#define req_queue_H

#include "request.hpp"

#include <queue>
#include <utility>

namespace queueing_system {
/**
 * @brief The RequestsQueue class - приоритетная очередь Заявок с двумя
 * приоритетами
 */
class RequestsQueue {
  std::queue<Request> q_first;
  std::queue<Request> q_second;

public:
  RequestsQueue() = default;

  RequestsQueue(const RequestsQueue&) = default;
  RequestsQueue& operator=(const RequestsQueue&) = default;

  RequestsQueue(RequestsQueue&&) noexcept = default;
  RequestsQueue& operator=(RequestsQueue&&) noexcept = default;

  ~RequestsQueue() = default;

  /**
   * @brief pop - аналогичный STL
   */
  void pop() {
    if (std::empty(q_first)) {
      q_second.pop();
    } else {
      q_first.pop();
    }
  }
  /**
   * @brief push  - аналогичный STL
   * @param request
   */
  void push(const Request& request) {
    if (request.type == RequestType::kSecond) {
      q_second.push(request);
    } else {
      q_first.push(request);
    }
  }
  /**
   * @brief clear - очистка очереди
   */
  void clear() {
    q_first = std::queue<Request>{};
    q_second = std::queue<Request>{};
  }
  /**
   * @brief front - аналогичный STL
   * @return
   */
  Request& front() noexcept {
    if (std::empty(q_first)) {
      return q_second.front();
    } else {
      return q_first.front();
    }
  }
  /**
   * @brief front - аналогичный STL
   * @return
   */
  const Request& front() const noexcept {
    return const_cast<RequestsQueue*>(this)->front();
  }
  /**
   * @brief empty - аналогичный STL
   * @return
   */
  bool empty() const noexcept {
    return std::empty(q_first) && std::empty(q_second);
  }
  /**
   * @brief status - количество заявок обоих типов в очереди
   */
  auto status() const noexcept {
    return std::make_pair<int, int>(std::size(q_first), std::size(q_second));
  }
};

} // namespace queueing_system

#endif // req_queue_H
