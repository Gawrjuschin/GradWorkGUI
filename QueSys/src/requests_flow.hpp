#ifndef req_flow_H
#define req_flow_H

#include "request.hpp"

#include <random>

namespace queueing_system {
/**
 * @brief The RequestsFlow class - потоки заявок обоих типов и поток
 * обслуживания. Независимые С.В.
 */
class RequestsFlow {
  // Три независимые С.В.
  std::default_random_engine arrive_engine_first{};
  std::default_random_engine arrive_engine_second{};
  std::default_random_engine maintenance_engine{};
  // Распределения для потоков заявок и потока обслуживания
  std::exponential_distribution<> arrive_distr_first;
  std::exponential_distribution<> arrive_distr_second;
  std::exponential_distribution<> maintenance_distr;

  double time{};               // время в системе для расчёта времени прибытия
  double arrive_time_first{};  // Время до следующей заявки первого типа
  double arrive_time_second{}; // Время до следующей заявки второго типа

public:
  RequestsFlow() = delete;

  RequestsFlow(double lambda, double mu, double prop)
      : arrive_distr_first{lambda * prop},
        arrive_distr_second{lambda * (1 - prop)}, maintenance_distr{mu} {
    std::random_device rd{};

    arrive_engine_first.seed(rd());
    arrive_engine_second.seed(rd());
    maintenance_engine.seed(rd());

    arrive_time_first = arrive_distr_first(arrive_engine_first);
    arrive_time_second = arrive_distr_second(arrive_engine_second);
  }

  RequestsFlow(const RequestsFlow&) = delete;
  RequestsFlow& operator=(const RequestsFlow&) = delete;

  RequestsFlow(RequestsFlow&&) = delete;
  RequestsFlow& operator=(RequestsFlow&&) = delete;

  ~RequestsFlow() = default;

  /**
   * @brief operator () - генерация очередной заявки
   * @param request_number - номер очередной заявки
   * @return
   */
  Request operator()(const std::uint32_t request_number) {
    // Заявка второго типа пришла раньше (приоритет 0)
    if (arrive_time_second <= arrive_time_first) {
      time += arrive_time_second;
      arrive_time_first -= arrive_time_second;
      arrive_time_second = arrive_distr_second(arrive_engine_second);

      return {.number = request_number,
              .type = RequestType::kSecond,
              .arrive_time = time,
              .serve_time = maintenance_distr(maintenance_engine)};
    }

    time += arrive_time_first;
    arrive_time_second -= arrive_time_first;
    arrive_time_first = arrive_distr_first(arrive_engine_first);

    return {.number = request_number,
            .type = RequestType::kFirst,
            .arrive_time = time,
            .serve_time = maintenance_distr(maintenance_engine)};
  }
};

} // namespace queueing_system

#endif // req_flow_H
