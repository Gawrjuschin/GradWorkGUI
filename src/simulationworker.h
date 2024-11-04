#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include "backendobjectworker.h"
#include "input_data.h"
#include "points_data.h"
#include "table_data.h"

#include <QThreadPool>

struct InputData;

struct SimulationData {
  PointsData points_data{};
  TableData table_data{};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimulationWorker : public BackendObjectWorker {
  Q_OBJECT
public:
  explicit SimulationWorker(const InputData& input_data,
                            QObject* parent = nullptr)
      : BackendObjectWorker(parent), r_input_data(input_data),
        p_data(new SimulationData), p_thread_pool(new QThreadPool(this)) {}
  ~SimulationWorker() override = default;

  PointsData& PointsData() noexcept { return p_data->points_data; }
  TableData& TableData() noexcept { return p_data->table_data; }

signals:
  void sigDone();

protected:
  void processBody() override;

private:
  const InputData& r_input_data;
  SimulationData* p_data;
  QThreadPool* p_thread_pool;

private:
  auto
  MakeSimulationPart(const std::pair<std::size_t, std::size_t>& points_range) {
    Q_ASSERT(points_range.first < points_range.second);
    Q_ASSERT(points_range.second <= PointsData::kPointsCount);

    return [this, start_point = points_range.first,
            end_point = points_range.second]() {
      constexpr auto dLoad =
          static_cast<double>(PointsData::kMaxLoad - PointsData::kMinLoad) /
          static_cast<long>(PointsData::kPointsCount);
      for (auto point_number = start_point;
           !canceled() && (point_number < end_point); ++point_number) {
        sleep();
        const auto load = PointsData::kMinLoad + dLoad * point_number;
        const auto lambda = load * r_input_data.mu * r_input_data.channels;

        const auto sim_result = queueing_system::Simulate(
            lambda, r_input_data.mu, r_input_data.channels,
            r_input_data.propability,
            queueing_system::MaxEventsCondition(r_input_data.events));
        p_data->points_data.load_result(point_number, load, sim_result);
        arrive();
      }
    };
  }
  auto MakeSimulationTable() noexcept {
    return [this] {
      auto& events = TableData().events;
      auto& requests = TableData().requests;
      events.clear();
      requests.clear();
      events.reserve(TableData::kEventsNumber);
      requests.reserve(TableData::kRequestsNumber);

      const auto lambda =
          PointsData::kMaxLoad * r_input_data.mu * r_input_data.channels;
      [[maybe_unused]] auto result = queueing_system::Simulate(
          lambda, r_input_data.mu, r_input_data.channels,
          r_input_data.propability,
          queueing_system::MaxEventsCondition(TableData::kEventsNumber),
          [&events](const Event& event) { events.push_back(event); },
          [&requests](const Request& request) { requests.push_back(request); });
    };
  }
};

inline void SimulationWorker::processBody() {
  p_thread_pool->setMaxThreadCount(r_input_data.threads);

  const std::size_t chunk_size =
      PointsData::kPointsCount / p_thread_pool->maxThreadCount();

  for (std::size_t thread_num{};
       thread_num < p_thread_pool->maxThreadCount() - 1; ++thread_num) {
    p_thread_pool->start(MakeSimulationPart(
        {chunk_size * thread_num, chunk_size * (thread_num + 1)}));
  }
  // Последний "кусок" вычислений с остатком
  p_thread_pool->start(
      MakeSimulationPart({chunk_size * (p_thread_pool->maxThreadCount() - 1),
                          PointsData::kPointsCount}));

  p_thread_pool->waitForDone();

  if (!canceled()) {
    p_thread_pool->start(MakeSimulationTable());

    p_data->points_data.process_results();

    p_thread_pool->waitForDone();
  }
}

#endif // SIMULATIONWORKER_H
