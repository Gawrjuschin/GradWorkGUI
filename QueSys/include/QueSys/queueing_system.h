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
  /**
   * @brief time_passed - время последнего события
   */
  double time_passed{};
  /**
   * @brief events - число событий за время симуляции (номер последнего события)
   */
  std::uint32_t events{};
  /**
   * @brief requests - число заявок за время симуляции (номер последней заявки)
   */
  std::uint32_t requests{};
  /**
   * @brief avg_period - среднее время между заявками каждого типа
   */
  std::pair<double, double> avg_period{};
  /**
   * @brief avg_serve - среднее время обслуживания заявок каждого типа
   */
  std::pair<double, double> avg_serve{};
  /**
   * @brief propability - доли заявок каждого типа
   */
  std::pair<double, double> propability{};
  /**
   * @brief avg_wait - среднее время ожидания в очереди заявок каждого типа
   */
  std::pair<double, double> avg_wait{};
  /**
   * @brief avg_utility - среднее время в СМО заявок каждого типа
   */
  std::pair<double, double> avg_utility{};
  /**
   * @brief avg_requests - среднее число заявок каждого типа
   */
  std::pair<double, double> avg_requests{};
};

/**
 * @brief The SimulationStatus class - состояние СМО в момент времени
 time_passed. Используется для проверки условия продолжения симуляции.
 */
struct SimulationStatus {
  /**
   * @brief time_passed - время последнего события
   */
  double time_passed{};
  /**
   * @brief events - число событий за время симуляции (номер последнего события)
   */
  std::uint32_t event_number{0};
  /**
   * @brief requests - число заявок за время симуляции (номер последней заявки)
   */
  std::uint32_t request_number{0};
  /**
   * @brief serve_total - общее время обслуживания заявок каждого типа
   */
  std::pair<double, double> serve_total{};
  /**
   * @brief wait_total - общее время ожидания в очереди заявок каждого типа
   */
  std::pair<double, double> wait_total{};
  /**
   * @brief requests_total - общее число заявок каждого типа
   */
  std::pair<int, int> requests_total{};
  /**
   * @brief requests_served - число обслуженных заявок каждого типа
   */
  std::pair<int, int> requests_served{};
  /**
   * @brief requests_waited - число заявок, стоявших в оченеди, каждого типа
   */
  std::pair<int, int> requests_waited{};
  /**
   * @brief requests_weighted_summ - общее число заявок каждого типа,
   * нормированное по времени между ними (взвешенное)
   */
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
class MaxEventsCondition {
  std::size_t max_events_;

public:
  constexpr explicit MaxEventsCondition(const std::size_t max_events) noexcept
      : max_events_(max_events) {}

  MaxEventsCondition(const MaxEventsCondition&) noexcept = default;
  MaxEventsCondition& operator=(const MaxEventsCondition&) noexcept = default;

  MaxEventsCondition(MaxEventsCondition&&) noexcept = default;
  MaxEventsCondition& operator=(MaxEventsCondition&&) noexcept = default;

  ~MaxEventsCondition() noexcept = default;

  /**
   * @brief operator () - условие продолжения
   * @param simulation_status - состояние СМО на текущей итерации симуляции
   * @return
   */
  constexpr bool
  operator()(const SimulationStatus& simulation_status) const noexcept {
    return simulation_status.event_number <= max_events_;
  }
};

/**
 * @brief The PropConvCondition class - условие продолжения симуляции по
 * сходимости теоретической доли заявок с заданной точностью eps.
 */
class PropConvCondition {
  mutable double prev_prop_{};
  double eps_;

public:
  /**
   * @brief PropConvCondition - конструктор
   * @param eps - точность
   */
  constexpr explicit PropConvCondition(const double eps) noexcept : eps_(eps) {}

  PropConvCondition(const PropConvCondition&) = default;
  PropConvCondition& operator=(const PropConvCondition&) = default;

  PropConvCondition(PropConvCondition&&) = default;
  PropConvCondition& operator=(PropConvCondition&&) = default;

  ~PropConvCondition() = default;

  /**
   * @brief operator () - условие продолжения
   * @param simulation_status - состояние СМО на текущей итерации симуляции
   * @return
   */
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
