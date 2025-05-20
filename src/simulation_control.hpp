#ifndef SIMULATION_CONTROL_H
#define SIMULATION_CONTROL_H

#include <QObject>
#include <QThread>
#include <QThreadPool>

#include "simulation_interface.hpp"

/**
 * @brief The SimulationControl class - интерфейс backend симуляции. Запускает
 * указанный BackendObjectWorker в отдельном треде. Позволяет управлять ходом
 * симуляции (старт, пауза и отмена). Предоставляет потокобезопасный счётчик
 * прогресса выполнения.
 */
class SimulationControl : public QObject {
  Q_OBJECT

public:
  explicit SimulationControl(SimulationInterface* worker,
                             QObject* parent = nullptr)
      : QObject(parent), p_worker_thread(new QThread(this)), p_worker(worker) {
    Q_ASSERT(p_worker);
    p_worker->moveToThread(p_worker_thread);
    p_worker_thread->start();

    connect(this, &SimulationControl::sigStarted, p_worker,
            &SimulationInterface::onProcess);
    connect(p_worker, &SimulationInterface::sigDone, this,
            &SimulationControl::sigDataReady);
    connect(p_worker, &SimulationInterface::sigStopped, this,
            &SimulationControl::sigStopped);
  }

  ~SimulationControl() override {
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
  void onStart() { emit sigStarted(); }
  void onPause() { p_worker->pause(); }
  void onResume() { p_worker->resume(); }
  void onStop() { p_worker->stop(); }

signals:
  void sigStarted();
  void sigDataReady();
  void sigStopped();

private:
  QThread* p_worker_thread;
  SimulationInterface* p_worker;
};

#endif // SIMULATION_CONTROL_H
