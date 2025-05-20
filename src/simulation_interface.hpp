#ifndef SIMULATION_INTERFACE_H
#define SIMULATION_INTERFACE_H

#include "progress.h"
#include "threadscontrol.h"

#include <QObject>

/**
 * @brief The SimulationInterface class - интерфейс симуляции с контролем
 * выполнения и расчётом прогресса.
 */
class SimulationInterface : public QObject {
  Q_OBJECT

  ThreadsControl threads_control_;
  Progress progress_{};

public:
  explicit SimulationInterface(QObject* parent = nullptr) : QObject(parent) {}
  virtual ~SimulationInterface() = default;

  const Progress& GetProgress() const noexcept { return progress_; }

  // Управление выполнением задания (не слоты!!!)
  void pause() { threads_control_.pause(); }
  void resume() { threads_control_.resume(); }
  void stop() { threads_control_.cancel(); }

  bool paused() const noexcept { return threads_control_.paused(); };
  bool canceled() const noexcept { return threads_control_.canceled(); }

protected:
  //
  /**
   * @brief arrive - Сигнал о выполненном куске processBody. Должен вызываться
   * при выполнении части симуляции для расчёта прогресса.
   */
  void arrive() noexcept { progress_.arrive(); }

  //
  /**
   * @brief sleep - Сон для реализации паузы. Вызывается перед частью симуляции
   * для поддержки паузы.
   */
  void sleep() const noexcept { threads_control_.sleep(); }

  /**
   * @brief processBody - Тело симуляции, определяемое пользователем
   */
  virtual void processBody() = 0;

public slots:
  void onProcess() {
    progress_.reset();
    threads_control_.reset();
    processBody();
    Q_ASSERT(!threads_control_.paused());

    if (threads_control_.canceled()) {
      emit sigStopped();
      return;
    }

    emit sigDone();
  }

signals:
  void sigDone();
  void sigStopped();
};

#endif // SIMULATION_INTERFACE_H
