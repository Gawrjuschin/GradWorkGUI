#ifndef req_queue_H
#define req_queue_H

#include "request.h"
#include <queue>

// TODO: заменить простой структурой
using int_pair = std::pair<int, int>;

class Req_queue//Класс приоритетной очереди с двумя приоритетами
{

public:
  Req_queue();

  void pop(); // Аналог метода pop() обычной очереди
  void push(const Request& ri); // Аналог метода push() обычной очереди
  Request front(); // Аналог метода front() обычной очереди
  bool empty(); // Аналог метода empty() обычной очереди

  int_pair status() const;
  void clear();

private:
  std::queue<Request> Q[2]; // Массив очередей
  int_pair qstatus; // Cостояние очереди

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

void Req_queue::push(const Request& request)//Перегрузка метода push() обычной очереди
{
  int ind = (request.type == 2);
  Q[ind].push(request);
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

int_pair Req_queue::status() const
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
