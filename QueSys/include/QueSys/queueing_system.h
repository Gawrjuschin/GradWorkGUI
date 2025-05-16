#ifndef queueing_system_H
#define queueing_system_H

#include "event.hpp"
#include "request.hpp"

#include <cmath>
#include <functional>
#include <utility>

namespace queueing_system {

/**
 * @brief The SimulationResult class - расчётные экспериментальные
 * параметры СМО.
 */
struct SimulationResult {
  double time_passed{};
  std::uint32_t events{};
  std::uint32_t requests{};
  std::pair<double, double> avg_period;
  std::pair<double, double> avg_serve;
  std::pair<double, double> propability;
  std::pair<double, double> avg_wait;
  std::pair<double, double> avg_utility;
  std::pair<double, double> avg_requests;
};

/**
 * @brief The SimulationStatus class - состояние СМО в момент времени
 time_passed. Используется для проверки условия продолжения симуляции.
 */
struct SimulationStatus {

  std::uint32_t request_number{0};
  std::uint32_t event_number{0};
  double time_passed{};
  std::pair<double, double> serve_total{};
  std::pair<double, double> wait_total{};
  std::pair<int, int> requests_total{};
  std::pair<int, int> requests_served{};
  std::pair<int, int> requests_waited{};
  std::pair<double, double> requests_weighted_summ{};
};

/**
 * @brief CalcResult - расчёт параметров СМО из её текущего состояния
 * @param simulation_status - состояние СМО
 * @return
 */
SimulationResult CalcResult(const SimulationStatus& simulation_status) noexcept;

namespace conditions {
/**
 * @brief The MaxEventsCondition class - условие продолжения симуляции по числу
 * событий. operator()  возвращает true, если условие выполняется
 */
struct MaxEventsCondition {
public:
  constexpr explicit MaxEventsCondition(const std::size_t max_events) noexcept
      : max_events_(max_events) {}

  MaxEventsCondition(const MaxEventsCondition&) noexcept = default;
  MaxEventsCondition& operator=(const MaxEventsCondition&) noexcept = default;

  MaxEventsCondition(MaxEventsCondition&&) noexcept = default;
  MaxEventsCondition& operator=(MaxEventsCondition&&) noexcept = default;

  ~MaxEventsCondition() noexcept = default;

  constexpr bool
  operator()(const SimulationStatus& simulation_status) const noexcept {
    return simulation_status.event_number <= max_events_;
  }

private:
  std::size_t max_events_;
};

/**
 * @brief The PropConvCondition class - условие продолжения симуляции по
 * сходимости теоретической доли заявок с заданной точностью eps
 */
struct PropConvCondition {
public:
  constexpr explicit PropConvCondition(const double eps) noexcept : eps_(eps) {}

  PropConvCondition(const PropConvCondition&) = default;
  PropConvCondition& operator=(const PropConvCondition&) = default;

  PropConvCondition(PropConvCondition&&) = default;
  PropConvCondition& operator=(PropConvCondition&&) = default;

  ~PropConvCondition() = default;

  // Условие продолжения
  constexpr bool
  operator()(const SimulationStatus& simulation_status) const noexcept {
    if (simulation_status.request_number == 0) {
      return true;
    }

    const auto res = CalcResult(simulation_status);
    if (prev_prop_ == res.propability.first ||
        std::abs(prev_prop_ - res.propability.first) > eps_) {
      prev_prop_ = res.propability.first;
      return true;
    }
    return false;
  }

private:
  mutable double prev_prop_{};
  double eps_;
};

} // namespace conditions

/**
 * @brief Simulate - запуск симуляции работы СМО с заданными параметрами
 * @param lambda_th - параметр интенсивности потока заявок СМО
 * @param mu_th - параметр интенсивности обслуживания СМО
 * @param channels_number - число каналов СМО
 * @param prop - доля заявок высшего приоритета в потоке заявок
 * @param continue_condition - Callable, предикат продолжения симуляции
 * @param write_event - Callable, записывающий все события
 * @param write_request - Callable, записывающий все заявки
 * @return
 */
SimulationResult
Simulate(double lambda_th, double mu_th, int channels_number, double prop,
         std::function<bool(const SimulationStatus&)> continue_condition,
         std::function<void(const Event&)> write_event,
         std::function<void(const Request&)> write_request);

/**
 * @brief Simulate - запуск симуляции работы СМО с заданными параметрами
 * @param lambda_th - параметр интенсивности потока заявок СМО
 * @param mu_th - параметр интенсивности обслуживания СМО
 * @param channels_number - число каналов СМО
 * @param prop - доля заявок высшего приоритета в потоке заявок
 * @param continue_condition - Callable, предикат продолжения симуляции
 * @return
 */
SimulationResult
Simulate(double lambda_th, double mu_th, int channels_number, double prop,
         std::function<bool(const SimulationStatus&)> continue_condition);

} // namespace queueing_system

#endif // queueing_system_H
