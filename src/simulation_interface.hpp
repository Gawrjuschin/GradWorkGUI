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

  /**
   * @brief getProgress - геттер прогресса
   * @return
   */
  const Progress& getProgress() const noexcept { return progress_; }

  /**
   * @brief pause - метод паузы выполнения чанков. Вызывается из слота в
   * SimulationControl.
   */
  void pause() { threads_control_.pause(); }
  /**
   * @brief resume - метод возобновления выполнения чанков. Вызывается из слота
   * в SimulationControl.
   */
  void resume() { threads_control_.resume(); }
  /**
   * @brief stop - метод остановки выполнения чанков. Вызывается из слота в
   * SimulationControl.
   */
  void stop() { threads_control_.cancel(); }

  /**
   * @brief paused - состояние флага паузы.
   * @return
   */
  bool paused() const noexcept { return threads_control_.paused(); };
  /**
   * @brief canceled - состояние флага отмены.
   * @return
   */
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
   * @brief processBody - Тело симуляции, определяемое пользователем.
   */
  virtual void processBody() = 0;

public slots:
  /**
   * @brief onProcess - вызов реализации тела симуляции с отправкой сигналов
   * после выполнения в соответствии с флагами паузы и отмены.
   */
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
  /**
   * @brief sigDone - сигнал о завершении симуляции. Отправляется из реализации
   * SimulationInterface::processBody().
   */
  void sigDone();
  /**
   * @brief sigStopped - сигнал об остановке. Отправляется из реализации
   * SimulationInterface::processBody().
   */
  void sigStopped();
};

#endif // SIMULATION_INTERFACE_H
