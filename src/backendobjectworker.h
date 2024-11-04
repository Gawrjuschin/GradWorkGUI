#ifndef BACKENDOBJECTWORKER_H
#define BACKENDOBJECTWORKER_H

#include <QObject>

#include "progress.h"
#include "threadscontrol.h"

// Интерфейс воркера
class BackendObjectWorker : public QObject {
  Q_OBJECT
public:
  explicit BackendObjectWorker(QObject* parent = nullptr) : QObject(parent) {}
  virtual ~BackendObjectWorker() = default;

  // const ThreadsControl& GetThreadsControl() const noexcept { return
  // *p_threads_control; };
  const Progress& GetProgress() const noexcept { return progress_; }

  // Управление выполнением задания (не слоты!!!)
  void pause() { threads_control_.pause(); }
  void resume() { threads_control_.resume(); }
  void stop() { threads_control_.cancel(); }

  bool paused() const noexcept { return threads_control_.paused(); };
  bool canceled() const noexcept { return threads_control_.canceled(); }

protected:
  // Методы, используемые для реализации останавливаемого воркера

  // Сигнал о выполненном куске processBody
  void arrive() noexcept { progress_.arrive(); }

  // Сон для реализации паузы
  void sleep() const noexcept { threads_control_.sleep(); }

  // Определённый наследником метод (кусок задачи)
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

private:
  ThreadsControl threads_control_;
  Progress progress_{};
};

#endif // BACKENDOBJECTWORKER_H
