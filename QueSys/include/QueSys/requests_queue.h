#ifndef req_queue_H
#define req_queue_H

#include "request.h"

#include <queue>
#include <utility>

// TODO: заменить простой структурой
using int_pair = std::pair<int, int>;

class RequestsQueue // Класс приоритетной очереди с двумя приоритетами
{
public:
  RequestsQueue() = default;

  RequestsQueue(const RequestsQueue&) = default;
  RequestsQueue& operator=(const RequestsQueue&) = default;

  RequestsQueue(RequestsQueue&&) noexcept = default;
  RequestsQueue& operator=(RequestsQueue&&) noexcept = default;

  ~RequestsQueue() = default;

  void pop();                   // Аналог метода pop() обычной очереди
  void push(const Request& ri); // Аналог метода push() обычной очереди
  void clear();

  Request& front() noexcept; // Аналог метода front() обычной очереди
  const Request&
  front() const noexcept; // Аналог метода front() обычной очереди

  bool empty() const noexcept; // Аналог метода empty() обычной очереди
  int_pair status() const noexcept;

private:
  std::queue<Request> q_first;
  std::queue<Request> q_second;
};

void RequestsQueue::pop() {
  if (std::empty(q_first)) {
    q_second.pop();
  } else {
    q_first.pop();
  }
}

void RequestsQueue::push(const Request& request) {
  if (request.type == RequestType::kSecond) {
    q_second.push(request);
  } else {
    q_first.push(request);
  }
}

void RequestsQueue::clear() {
  q_first = std::queue<Request>{};
  q_second = std::queue<Request>{};
}

Request& RequestsQueue::front() noexcept {
  if (std::empty(q_first)) {
    return q_second.front();
  } else {
    return q_first.front();
  }
}

const Request& RequestsQueue::front() const noexcept {
  return const_cast<RequestsQueue*>(this)->front();
}

bool RequestsQueue::empty() const noexcept {
  return std::empty(q_first) && std::empty(q_second);
}

int_pair RequestsQueue::status() const noexcept {
  return std::make_pair<int, int>(std::size(q_first), std::size(q_second));
}

#endif // req_queue_H
