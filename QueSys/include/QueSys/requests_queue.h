#ifndef req_queue_H
#define req_queue_H

#include "request.h"
#include <queue>

// TODO: заменить простой структурой
using int_pair = std::pair<int, int>;

class RequestsQueue //Класс приоритетной очереди с двумя приоритетами
{
public:
  RequestsQueue();

  void pop();                   // Аналог метода pop() обычной очереди
  void push(const Request& ri); // Аналог метода push() обычной очереди
  void clear();

  Request& front() noexcept; // Аналог метода front() обычной очереди
  const Request& front() const noexcept; // Аналог метода front() обычной очереди

  bool empty() const noexcept; // Аналог метода empty() обычной очереди
  int_pair status() const noexcept;

private:
  std::queue<Request> Q[2]; // Массив очередей
  int_pair qstatus;         // Cостояние очереди
};

RequestsQueue::RequestsQueue()
    : qstatus(0, 0)
{}

void RequestsQueue::pop()
{
  //Считаем, что очередь непустая
  int ind = Q[0].empty();
  Q[ind].pop();
  ind ? --qstatus.second : --qstatus.first;
}

void RequestsQueue::push(const Request& request)
{
  int ind = (request.type == RequestType::kSecond);
  Q[ind].push(request);
  ind ? ++qstatus.second : ++qstatus.first;
}

void RequestsQueue::clear()
{
  qstatus = {0, 0};
  Q[0] = std::queue<Request>{};
  Q[1] = std::queue<Request>{};
}

Request& RequestsQueue::front() noexcept
{
  return Q[0].empty() ? Q[1].front() : Q[0].front();
}

const Request& RequestsQueue::front() const noexcept
{
  return Q[0].empty() ? Q[1].front() : Q[0].front();
}

bool RequestsQueue::empty() const noexcept
{
  return Q[0].empty() && Q[1].empty();
}

int_pair RequestsQueue::status() const noexcept
{
  return qstatus;
}

#endif // req_queue_H
