#ifndef BACKENDOBJECT_H
#define BACKENDOBJECT_H

#include <QObject>
#include <QThread>
#include <QThreadPool>

#include <memory>

#include "QueSys/queueing_system.h"
#include "graphs_data.h"
#include "input_data.h"
// #include "progress.h"
#include "src/models.h"
#include "table_data.h"
#include "threadscontrol.h"

struct InputData;

struct SimulationData
{
  Graphs_Data points_data{};
  Table_Data table_data{};
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
  void slot_process()
  {
    p_progress->reset();
    process_body();
    p_threads_control->reset();
    emit signal_done();
  }

signals:
  void signal_done();

protected:
  void arrive() noexcept { p_progress->arrive(); }

  void sleep() const noexcept { p_threads_control->sleep(); }

  virtual void process_body() = 0;

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

  Graphs_Data* PointsData() noexcept { return std::addressof(p_data->points_data); }
  Table_Data* TableData() noexcept { return std::addressof(p_data->table_data); }

public slots:
  void process_body() override;

signals:
  void signal_done();

private:
  const InputData& r_input_data;
  SimulationData* p_data;
  QThreadPool* p_thread_pool;

private:
  auto MakeSimulationPart(const std::pair<std::size_t, std::size_t>& points_range)
  {
    Q_ASSERT(points_range.first < points_range.second);
    Q_ASSERT(points_range.second <= Graphs_Data::POINTS_COUNT);

    return [this, start_point = points_range.first, end_point = points_range.second]() {
      // TODO: продумать приведения типов и перенести в Graphs_Data(PointsData)
      constexpr auto dLoad = static_cast<double>(Graphs_Data::max_load - Graphs_Data::min_load)
			     / static_cast<long>(Graphs_Data::GRAPHS_COUNT);
      for (auto point_number = start_point; point_number < end_point; ++point_number) {
	const auto lambda = (Graphs_Data::min_load + dLoad * point_number)
			    * (r_input_data.mu * r_input_data.channels);

	const auto sim_result = queueing_system::Simulate(lambda,
							  r_input_data.mu,
							  r_input_data.channels,
							  r_input_data.propability,
							  queueing_system::MaxEventsCondition(
							      r_input_data.events));
	p_data->points_data.load(point_number, sim_result);
	arrive();
      }
    };
  }
  auto MakeSimulationTable()
  {
    return [this] {
      QVector<Event> events{};
      QVector<Request> requests{};
      events.reserve(Table_Data::events_number);
      requests.reserve(Table_Data::events_number / 2);

      const auto lambda = Graphs_Data::max_load * r_input_data.mu * r_input_data.channels;
      [[maybe_unused]] auto result = queueing_system::Simulate(
	  lambda,
	  r_input_data.mu,
	  r_input_data.channels,
	  r_input_data.propability,
	  queueing_system::MaxEventsCondition(Table_Data::events_number),
	  [&events](const Event& event) { events.push_back(event); },
	  [&requests](const Request& request) { requests.push_back(request); });

      // TODO: сделать метод replace
      p_data->table_data.event_model()->clear();
      p_data->table_data.request_model()->clear();

      p_data->table_data.event_model()->append(std::move(events));
      p_data->table_data.request_model()->append(std::move(requests));
    };
  }
};

inline void SimulationWorker::process_body()
{
  p_thread_pool->setMaxThreadCount(r_input_data.threads);

  const std::size_t chunk_size = Graphs_Data::POINTS_COUNT / p_thread_pool->maxThreadCount();

  for (std::size_t thread_num{}; thread_num < p_thread_pool->maxThreadCount() - 1; ++thread_num) {
    p_thread_pool->start(
	MakeSimulationPart({chunk_size * thread_num, chunk_size * (thread_num + 1)}));
  }
  // Последний "кусок" вычислений с остатком
  p_thread_pool->start(MakeSimulationPart(
      {chunk_size * (p_thread_pool->maxThreadCount() - 1), Graphs_Data::POINTS_COUNT}));

  p_thread_pool->waitForDone();

  if (!canceled()) {
    p_thread_pool->start(MakeSimulationTable());

    p_data->points_data.calc_Yranges();
    p_data->points_data.update();

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

    connect(this, &BackendObject::signal_start, p_worker, &BackendObjectWorker::slot_process);
    connect(p_worker, &BackendObjectWorker::signal_done, this, &BackendObject::slot_done);
  }

  ~BackendObject() override
  {
    if (p_worker_thread->isRunning()) {
      slot_stop();
      while (p_worker->canceled()) {
      }
    }
    p_worker_thread->exit();
    p_worker_thread->wait();
  }

  const Progress& getProgress() const noexcept { return p_worker->GetProgress(); }

public slots:
  void slot_start()
  {
    Q_ASSERT(!p_worker->canceled());
    Q_ASSERT(!p_worker->paused());
    emit signal_start();
  }
  void slot_pause() { p_worker->pause(); }
  void slot_resume() { p_worker->resume(); }
  void slot_stop() { p_worker->stop(); }
  void slot_done()
  {
    // TODO: расчитать новые границы
    emit signal_done();
  }

signals:
  void signal_start();
  void signal_done();

private:
  QThread* p_worker_thread;
  BackendObjectWorker* p_worker;
};

#endif // BACKENDOBJECT_H
