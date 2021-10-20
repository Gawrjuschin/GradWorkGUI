#ifndef req_flow_H
#define req_flow_H

#include <random>
#include <array>

#include "request_info.h"

class Req_flow
{ //Класс потока заявок
  //Генерирует информацию о следующей заявке
public:
  Req_flow( ) = delete;
  Req_flow(double lambda, double mu, double prop);
  ~Req_flow( );

  Request_info operator ()();

private:
  //Три независимые С.В.
  std::default_random_engine engine_l1;
  std::default_random_engine engine_l2;
  std::default_random_engine engine_m;
  //Распределения для потоков заявок и потока обслуживания
  std::exponential_distribution<> distr_l1;
  std::exponential_distribution<> distr_l2;
  std::exponential_distribution<> distr_m;
  double time;//время в системе для расчёта времени прибытия
  std::array<double, 2> arrive_time;//Время до следующей заявки соответствующего типа
  int priority;//Приоритет заявки (0 или 1). НЕ ПУТАТЬ С ТИПОМ ЗАЯВКИ!!!

private:
  double get_arrive(int i);
  //Метод для генерации семейства с. в.

  double get_serve();

};

Req_flow::Req_flow(double lambda, double mu, double prop)
  : engine_l1{}, engine_l2{}, engine_m{}
  , distr_l1{lambda * prop},  distr_l2{lambda * (1 - prop)}
  , distr_m{mu},  time{0},  priority{-1}
{
  arrive_time[0] = get_arrive(0);
  arrive_time[1] = get_arrive(1);
}

Req_flow::~Req_flow() = default;

Request_info Req_flow::operator ()()
{
  Request_info next_req;
  priority = arrive_time[0] < arrive_time[1] ? 0 : 1;
  time += arrive_time[ priority ];
  arrive_time[ (priority==0 ? 1 : 0) ] -= arrive_time[ priority ];
  arrive_time[ priority ] = get_arrive(priority);
  next_req.type = priority + 1;
  //ЗАЯВКИ БЫВАЮТ ПЕРВОГО ИЛИ ВТОРОГО ТИПА, А ПРИОРИТЕТ - 0 или 1
  next_req.arrive_time = time;
  next_req.serve_time = get_serve();
  return next_req;
}

double Req_flow::get_arrive(int i)
//Метод для генерации семейства с. в.
{
  return (i==0) ? distr_l1(engine_l1) : distr_l2(engine_l2);
}

double Req_flow::get_serve()
{
  return distr_m(engine_m);
}

#endif // req_flow_H
