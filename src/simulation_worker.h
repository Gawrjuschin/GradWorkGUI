#ifndef SIMULATION_WORKER_H
#define SIMULATION_WORKER_H

#include "simulation_interface.hpp"
#include "input_data.h"
#include "points_data.h"
#include "table_data.h"

#include <QThreadPool>

struct InputData;

struct SimulationData {
  PointsData points_data{};
  TableData table_data{};
};

/**
 * @brief The SimulationWorker class - реализация многопоточной симуляции СМО с
 * заданными параметрами
 */
class SimulationWorker : public SimulationInterface {
  Q_OBJECT

  const InputData& r_input_data;
  QThreadPool* p_thread_pool;
  SimulationData m_data{};

public:
  explicit SimulationWorker(const InputData& input_data,
                            QObject* parent = nullptr)
      : SimulationInterface(parent), r_input_data(input_data),
        p_thread_pool(new QThreadPool(this)) {
    m_data.table_data.events.reserve(TableData::kEventsNumber);
    m_data.table_data.requests.reserve(TableData::kRequestsNumber);
  }
  ~SimulationWorker() override = default;

  PointsData& PointsData() noexcept { return m_data.points_data; }
  TableData& TableData() noexcept { return m_data.table_data; }

signals:
  void sigDone();

protected:
  void processBody() override;

private:
  auto MakeSimulationPartTask(const int start_point, const int end_point) {
    Q_ASSERT(start_point < end_point);
    Q_ASSERT(end_point <= PointsData::kPointsCount);

    return [this, start_point, end_point]() {
      constexpr auto dLoad =
          static_cast<double>(PointsData::kMaxLoad - PointsData::kMinLoad) /
          static_cast<int>(PointsData::kPointsCount);
      for (auto point_number = start_point;
           !canceled() && (point_number < end_point); ++point_number) {
        sleep();
        const auto load = PointsData::kMinLoad + dLoad * point_number;
        const auto lambda = load * r_input_data.mu * r_input_data.channels;

        const auto sim_result = queueing_system::Simulate(
            lambda, r_input_data.mu, r_input_data.channels,
            r_input_data.propability,
            queueing_system::conditions::MaxEventsCondition(
                r_input_data.events));
        m_data.points_data.load_result(point_number, load, sim_result);
        arrive();
      }
    };
  }
  auto MakeSimulationTableTask() noexcept {
    return [this] {
      auto& events = TableData().events;
      auto& requests = TableData().requests;
      events.clear();
      requests.clear();

      const auto lambda =
          PointsData::kMaxLoad * r_input_data.mu * r_input_data.channels;
      [[maybe_unused]] auto result = queueing_system::Simulate(
          lambda, r_input_data.mu, r_input_data.channels,
          r_input_data.propability,
          queueing_system::conditions::MaxEventsCondition(
              TableData::kEventsNumber),
          [&events](const queueing_system::Event& event) {
            events.push_back(event);
          },
          [&requests](const queueing_system::Request& request) {
            requests.push_back(request);
          });
    };
  }

  auto MakeGraphsPrepareTask() noexcept {
    return [this]() { m_data.points_data.process_results(); };
  }
};

inline void SimulationWorker::processBody() {
  p_thread_pool->setMaxThreadCount(r_input_data.threads);

  const auto chunk_size = PointsData::kPointsCount / r_input_data.threads;

  for (int thread_num{}; thread_num < r_input_data.threads - 1; ++thread_num) {
    p_thread_pool->start(MakeSimulationPartTask(chunk_size * thread_num,
                                                chunk_size * (thread_num + 1)));
  }
  // Последний "кусок" вычислений с остатком
  p_thread_pool->start(MakeSimulationPartTask(
      chunk_size * (r_input_data.threads - 1), PointsData::kPointsCount));

  p_thread_pool->waitForDone();

  // Заполнение таблицы и подготовка графов
  if (!canceled()) {
    p_thread_pool->start(MakeSimulationTableTask());
    p_thread_pool->start(MakeGraphsPrepareTask());

    p_thread_pool->waitForDone();
  }
}

#endif // SIMULATION_WORKER_H
