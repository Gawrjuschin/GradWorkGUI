#ifndef req_flow_H
#define req_flow_H

#include <array>
#include <random>

#include "request.h"

class RequestsFlow
{ //Класс потока заявок
  //Генерирует информацию о следующей заявке
public:
  RequestsFlow() = delete;
  RequestsFlow(double lambda, double mu, double prop);

  RequestsFlow(const RequestsFlow&) = delete;
  RequestsFlow& operator=(const RequestsFlow&) = delete;

  RequestsFlow(RequestsFlow&&) = delete;
  RequestsFlow& operator=(RequestsFlow&&) = delete;

  ~RequestsFlow();

  Request operator()(int request_number);

private:
  //Три независимые С.В.
  std::default_random_engine engine_l1;
  std::default_random_engine engine_l2;
  std::default_random_engine engine_m;
  //Распределения для потоков заявок и потока обслуживания
  std::exponential_distribution<> distr_l1;
  std::exponential_distribution<> distr_l2;
  std::exponential_distribution<> distr_m;
  double time; //время в системе для расчёта времени прибытия
  std::array<double, 2> arrive_time{}; //Время до следующей заявки соответствующего типа
  int priority; //Приоритет заявки (0 или 1). НЕ ПУТАТЬ С ТИПОМ ЗАЯВКИ!!!

private:
  double get_arrive(int i);
  //Метод для генерации семейства с. в.

  double get_serve();
};

RequestsFlow::RequestsFlow(double lambda, double mu, double prop)
    : engine_l1{}
    , engine_l2{}
    , engine_m{}
    , distr_l1{lambda * prop}
    , distr_l2{lambda * (1 - prop)}
    , distr_m{mu}
    , time{0}
    , priority{-1}
{
  arrive_time[0] = get_arrive(0);
  arrive_time[1] = get_arrive(1);
}

RequestsFlow::~RequestsFlow() = default;

Request RequestsFlow::operator()(int request_number)
{
  Request next_req{};
  // TODO: отрефакторить, чтобы не было индексов
  priority = arrive_time[0] < arrive_time[1] ? 0 : 1;
  time += arrive_time[priority];
  arrive_time[(priority + 1) % 2] -= arrive_time[priority];
  arrive_time[priority] = get_arrive(priority);
  next_req.number = request_number;
  next_req.type = RequestType{priority + 1};
  //ЗАЯВКИ БЫВАЮТ ПЕРВОГО ИЛИ ВТОРОГО ТИПА, А ПРИОРИТЕТ - 0 или 1
  next_req.arrive_time = time;
  next_req.serve_time = get_serve();
  return next_req;
}

double RequestsFlow::get_arrive(int i)
//Метод для генерации семейства с. в.
{
  return (i == 0) ? distr_l1(engine_l1) : distr_l2(engine_l2);
}

double RequestsFlow::get_serve()
{
  return distr_m(engine_m);
}

#endif // req_flow_H
