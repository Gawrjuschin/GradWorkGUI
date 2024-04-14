#ifndef BACKENDOBJECT_H
#define BACKENDOBJECT_H

#include <QObject>
#include <QThread>
#include <QThreadPool>

#include <memory>

#include "QueSys/queueing_system.h"
#include "input_data.h"
#include "points_data.h"
// #include "progress.h"
#include "table_data.h"
#include "threadscontrol.h"

struct InputData;

struct SimulationData
{
  PointsData points_data{};
  TableData table_data{};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Интерфейс воркера
class BackendObjectWorker : public QObject
{
  Q_OBJECT
public:
  explicit BackendObjectWorker(QObject* parent = nullptr)
      : p_threads_control(std::make_unique<ThreadsControl>())
      , p_progress(std::make_unique<Progress>())
  {
    // Как определить, что ведутся вычисления???
  }

  virtual ~BackendObjectWorker() = default;

  // const ThreadsControl& GetThreadsControl() const noexcept { return *p_threads_control; };
  const Progress& GetProgress() const noexcept { return *p_progress; }

  // Управление выполнением задания
  void pause() { p_threads_control->pause(); }
  void resume() { p_threads_control->resume(); }
  void stop() { p_threads_control->cancel(); }

  bool paused() const noexcept { return p_threads_control->paused(); };
  bool canceled() const noexcept { return p_threads_control->canceled(); }

public slots:
  void onProcess() {
    p_progress->reset();
    processBody();
    p_threads_control->reset();
    emit sigDone();
  }

signals:
  void sigDone();

protected:
  void arrive() noexcept { p_progress->arrive(); }

  void sleep() const noexcept { p_threads_control->sleep(); }

  virtual void processBody() = 0;

private:
  std::unique_ptr<ThreadsControl> p_threads_control;
  std::unique_ptr<Progress> p_progress;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SimulationWorker : public BackendObjectWorker
{
  Q_OBJECT
public:
  explicit SimulationWorker(const InputData& input_data, QObject* parent = nullptr)
      : BackendObjectWorker(parent)
      , r_input_data(input_data)
      , p_data(new SimulationData)
      , p_thread_pool(new QThreadPool(this))
  {}
  ~SimulationWorker() override {}

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
  auto MakeSimulationPart(const std::pair<std::size_t, std::size_t>& points_range)
  {
    Q_ASSERT(points_range.first < points_range.second);
    Q_ASSERT(points_range.second <= PointsData::kPointsCount);

    return [this, start_point = points_range.first,
            end_point = points_range.second]() {
      constexpr auto dLoad =
          static_cast<double>(PointsData::kMaxLoad - PointsData::kMinLoad) /
          static_cast<long>(PointsData::kPointsCount);
      for (auto point_number = start_point; point_number < end_point; ++point_number) {
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
  auto MakeSimulationTable()
  {
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

  for (std::size_t thread_num{}; thread_num < p_thread_pool->maxThreadCount() - 1; ++thread_num) {
    p_thread_pool->start(
	MakeSimulationPart({chunk_size * thread_num, chunk_size * (thread_num + 1)}));
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BackendObject : public QObject
{
  Q_OBJECT

public:
  explicit BackendObject(BackendObjectWorker* worker, QObject* parent = nullptr)
      : p_worker_thread(new QThread(this))
      , p_worker(worker)
  {
    Q_ASSERT(p_worker);
    p_worker->moveToThread(p_worker_thread);
    p_worker_thread->start();

    connect(this, &BackendObject::sigStart, p_worker,
            &BackendObjectWorker::onProcess);
    connect(p_worker, &BackendObjectWorker::sigDone, this,
            &BackendObject::onDone);
  }

  ~BackendObject() override
  {
    if (p_worker_thread->isRunning()) {
      onStop();
      while (p_worker->canceled()) {
      }
    }
    p_worker_thread->exit();
    p_worker_thread->wait();
  }

  const Progress& getProgress() const noexcept { return p_worker->GetProgress(); }

public slots:
  void onStart() {
    Q_ASSERT(!p_worker->canceled());
    Q_ASSERT(!p_worker->paused());
    emit sigStart();
  }
  void onPause() { p_worker->pause(); }
  void onResume() { p_worker->resume(); }
  void onStop() { p_worker->stop(); }
  void onDone() {
    // TODO: расчитать новые границы
    emit sigDataReady();
  }

signals:
  void sigStart();
  void sigDataReady();

private:
  QThread* p_worker_thread;
  BackendObjectWorker* p_worker;
};

#endif // BACKENDOBJECT_H
