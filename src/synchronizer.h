#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include "progress.h"

#include <QAtomicInt>
#include <atomic>
#include <memory>

class QMutex;
class QWaitCondition;
class QAtomicInt;

class Synchronizer
{
public:
  Synchronizer();
  ~Synchronizer();

  void sleep();

  void pause();
  bool paused() const;
  void resume();

  void cancel();
  bool canceled() const;

  void resetCancel();
  void resetBarrier();

  int operator++();
  int operator++(int);

  int getThreadNum() const;
  void setThreadNum(int thrnum);

  Progress& getProgress() noexcept;
  const Progress& getProgress() const noexcept;

private:
  std::unique_ptr<QMutex> p_mutex;
  std::unique_ptr<QWaitCondition> p_condVar;
  std::atomic_bool m_canceled;
  QAtomicInt m_barrier;
  Progress m_progress;
  bool m_pause{false};
  int m_thr_num{0};
};

#endif // SYNCHRONIZER_H
