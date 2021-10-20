#ifndef req_queue_H
#define req_queue_H

#include "request_info.h"
#include <queue>

using ipair = std::pair<int, int>;
using Request = std::pair<int, Request_info>;

class Req_queue//Класс приоритетной очереди с двумя приоритетами
{

public:
  Req_queue();

  void pop(); // Аналог метода pop() обычной очереди
  void push(const Request& ri); // Аналог метода push() обычной очереди
  Request front(); // Аналог метода front() обычной очереди
  bool empty(); // Аналог метода empty() обычной очереди

  ipair status() const;
  void clear();

private:
  std::queue<Request> Q[2]; // Массив очередей
  ipair qstatus; // Cостояние очереди

};


Req_queue::Req_queue()
  :qstatus(0,0)
{

}

void Req_queue::pop()//Перегрузка метода pop() обычной очереди
{
  //Считаем, что очередь непустая
  int ind = Q[0].empty();
  Q[ind].pop();
  ind ? --qstatus.second : --qstatus.first;
}

void Req_queue::push(const Request& ri)//Перегрузка метода push() обычной очереди
{
  int ind = (ri.second.type == 2);
  Q[ind].push(ri);
  ind ? ++qstatus.second : ++qstatus.first;
}

Request Req_queue::front()//Перегрузка метода front() обычной очереди
{
  return Q[0].empty() ? Q[1].front() : Q[0].front();
}

bool Req_queue::empty()//Перегрузка метода empty() обычной очереди
{
  return Q[0].empty() && Q[1].empty();
}

ipair Req_queue::status() const
{
  return qstatus;
}

void Req_queue::clear()
{
    qstatus = {0,0};
    Q[0] = std::queue<Request>{};
    Q[1] = std::queue<Request>{};
}

#endif // req_queue_H
