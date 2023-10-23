#include "synchronizer.h"
#include "progress.h"

#include <QMutex>
#include <QWaitCondition>

Synchronizer::Synchronizer( )
  : p_mutex(new QMutex)
  , p_condVar(new QWaitCondition)
  , m_canceled ATOMIC_FLAG_INIT
  , m_barrier(0)
{

}

Synchronizer::~Synchronizer( ) = default;

void Synchronizer::sleep()
{
  p_mutex->lock();
  if(m_pause)
    {
      p_condVar->wait(p_mutex.get());
    }
  p_mutex->unlock();
}

void Synchronizer::pause()
{
  p_mutex->lock();
  m_pause = !m_pause;
  p_mutex->unlock();
}

bool Synchronizer::paused() const
{
  return m_pause;
}

void Synchronizer::resume()
{
  p_mutex->lock();
  m_pause = !m_pause;
  p_mutex->unlock();
  p_condVar->wakeAll();
}

void Synchronizer::cancel()
{
  while(!m_canceled.exchange(true)) { }
  m_pause = false;
  p_condVar->wakeAll();
}

bool Synchronizer::canceled() const
{
  return m_canceled.load();
}

void Synchronizer::resetCancel()
{
  m_canceled.store(false);
}

void Synchronizer::resetBarrier()
{
  m_barrier = 0;
}

int Synchronizer::operator ++()
{
  return ++m_barrier;
}

int Synchronizer::operator ++(int)
{
  return m_barrier++;
}

int Synchronizer::getThreadNum() const
{
  return m_thr_num;
}

void Synchronizer::setThreadNum(int thrnum)
{
  m_thr_num = thrnum;
}

Progress&  Synchronizer::getProgress() noexcept
{
  return m_progress;
}

const Progress&  Synchronizer::getProgress() const noexcept
{
  return m_progress;
}
