#ifndef queueing_system_H
#define queueing_system_H

#include "event.h"
#include "request.h"
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace queueing_system {

struct SimulationResult
{
  double time_passed{};
  int events{};
  int requests{};
  std::pair<double, double> avg_period;
  std::pair<double, double> avg_serve;
  std::pair<double, double> propability;
  std::pair<double, double> avg_wait;
  std::pair<double, double> avg_utility;
  std::pair<double, double> avg_requests;
};

// TODO: убрать SimulationStatus и заменить его в API на SimulationResult
struct SimulationStatus
{
  // Номер очередной заявки. Фактически количество сгенерированных заявок
  int request_number{0};
  // Номер последнего события. Фактически количество сгенерированных событий
  int event_number{0};

  double time_passed{};
  std::pair<double, double> serve_total{};
  std::pair<double, double> wait_total{};
  std::pair<int, int> requests_total{};
  std::pair<int, int> requests_served{};
  std::pair<int, int> requests_waited{};
  // Среднее число заяволк в системе вычисляется на ходу
  std::pair<double, double> requests_weighted_summ{};
};

// TODO: заменить на отдельные функции для расчёта характеристик
SimulationResult CalcResult(const SimulationStatus& simulation_status) noexcept;

// Выход по достижению указанного числа событий
struct MaxEventsCondition
{
public:
  MaxEventsCondition(const std::size_t max_events)
      : max_events_(max_events)
  {}

  MaxEventsCondition(const MaxEventsCondition&) = default;
  MaxEventsCondition& operator=(const MaxEventsCondition&) = default;

  MaxEventsCondition(MaxEventsCondition&&) = default;
  MaxEventsCondition& operator=(MaxEventsCondition&&) = default;

  ~MaxEventsCondition() = default;

  // Условие продолжения
  constexpr bool operator()(const SimulationStatus& simulation_status) const noexcept
  {
    return simulation_status.event_number <= max_events_;
  }

private:
  std::size_t max_events_;
};

// Выход по сходимости по доли заявок
struct PropConvCondition
{
public:
  PropConvCondition(const double eps)
      : eps_(eps)
  {}

  PropConvCondition(const PropConvCondition&) = default;
  PropConvCondition& operator=(const PropConvCondition&) = default;

  PropConvCondition(PropConvCondition&&) = default;
  PropConvCondition& operator=(PropConvCondition&&) = default;

  ~PropConvCondition() = default;

  // Условие продолжения
  // TODO: протестировать
  constexpr bool operator()(const SimulationStatus& simulation_status) const noexcept
  {
    if (simulation_status.request_number == 0) {
      return true;
    }

    auto res = CalcResult(simulation_status);
    if (prev_prop_ == res.propability.first || std::abs(prev_prop_ - res.propability.first) > eps_) {
      prev_prop_ = res.propability.first;
      return true;
    }
    return false;
  }

private:
  mutable double prev_prop_{};
  double eps_;
};

SimulationResult Simulate(double lambda_th,
			  double mu_th,
			  int channels_number,
			  double prop,
			  std::function<bool(const SimulationStatus&)> continue_condition,
			  std::function<void(const Event&)> write_event = nullptr,
			  std::function<void(const Request&)> write_request = nullptr);
// TODO: реализовать перегрузку с ограничением на eps

} // namespace queueing_system

#endif // queueing_system_H
