#ifndef BACKENDOBJECT_H
#define BACKENDOBJECT_H

#include <QObject>
#include <QThread>
#include <QThreadPool>

#include "backendobjectworker.h"

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

    connect(this, &BackendObject::sigStarted, p_worker,
            &BackendObjectWorker::onProcess);
    connect(p_worker, &BackendObjectWorker::sigDone, this,
            &BackendObject::sigDataReady);
    connect(p_worker, &BackendObjectWorker::sigStopped, this,
            &BackendObject::sigStopped);
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
  BackendObjectWorker* p_worker;
};

#endif // BACKENDOBJECT_H
