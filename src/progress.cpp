#include "progress.h"

#include <QAtomicInt>

#include <QDebug>

Progress::~Progress( ) = default;

int Progress::operator ++( )
{
  return m_progress++;
}

int Progress::operator ++( int )
{
  return ++m_progress;
}

int Progress::getProgress() const noexcept
{
  return m_progress.loadRelaxed();
}

void Progress::resetProgress()
{
  m_progress = 0;
}
