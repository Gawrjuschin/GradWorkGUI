#ifndef req_flow_H
#define req_flow_H

#include <array>
#include <random>

#include "request.h"

class RequestsFlow { // Класс потока заявок
                     // Генерирует информацию о следующей заявке
public:
  RequestsFlow() = delete;
  RequestsFlow(double lambda, double mu, double prop,
               const std::uint32_t seed = {});

  RequestsFlow(const RequestsFlow&) = delete;
  RequestsFlow& operator=(const RequestsFlow&) = delete;

  RequestsFlow(RequestsFlow&&) = delete;
  RequestsFlow& operator=(RequestsFlow&&) = delete;

  ~RequestsFlow();

  Request operator()(int request_number);

private:
  // Три независимые С.В.
  std::default_random_engine engine_l1{};
  std::default_random_engine engine_l2{};
  std::default_random_engine engine_m{};
  // Распределения для потоков заявок и потока обслуживания
  std::exponential_distribution<> distr_l1;
  std::exponential_distribution<> distr_l2;
  std::exponential_distribution<> distr_m;
  double time; // время в системе для расчёта времени прибытия
  std::array<double, 2>
      arrive_time{}; // Время до следующей заявки соответствующего типа
  // int priority; //Приоритет заявки (0 или 1). НЕ ПУТАТЬ С ТИПОМ ЗАЯВКИ!!!

private:
  double get_arrive(int i);
  // Метод для генерации семейства с. в.

  double get_serve();
};

RequestsFlow::RequestsFlow(double lambda, double mu, double prop,
                           const std::uint32_t seed)
    : distr_l1{lambda * prop}, distr_l2{lambda * (1 - prop)}, distr_m{mu},
      time{0} // , priority{-1}
{
  std::default_random_engine seed_engine{seed};
  std::uniform_int_distribution seed_distr{};

  engine_l1.seed(seed_distr(seed_engine));
  engine_l2.seed(seed_distr(seed_engine));
  engine_m.seed(seed_distr(seed_engine));

  arrive_time[0] = get_arrive(0);
  arrive_time[1] = get_arrive(1);
}

RequestsFlow::~RequestsFlow() = default;

Request RequestsFlow::operator()(int request_number) {
  // Заявки типа 1 имеют высший приоритет (0), а заявки типа 0 - низший (1)
  const auto priority = int(arrive_time[0] >= arrive_time[1]);
  const auto type = (priority + 1) % 2; // 0 или 1
  time += arrive_time[priority];
  arrive_time[type] -= arrive_time[priority];
  arrive_time[priority] = get_arrive(priority);
  return {.number = request_number,
          .type = RequestType{type + 1}, // Enum начинается с 1 (0 это None)
          .arrive_time = time,
          .serve_time = get_serve()};
}

double RequestsFlow::get_arrive(int i)
// Метод для генерации семейства с. в.
{
  return (i == 0) ? distr_l1(engine_l1) : distr_l2(engine_l2);
}

double RequestsFlow::get_serve() { return distr_m(engine_m); }

#endif // req_flow_H
