#include "Backend/progress.h"

#include <QAtomicInt>

#include <QDebug>

Progress::Progress( )
  : p_progress( new QAtomicInt(0) )
{

}

Progress::~Progress( ) = default;

int Progress::operator ++( )
{
  return (*p_progress)++;
}

int Progress::operator ++( int )
{
  return ++(*p_progress);
}

int Progress::getProgress()
{
  return *p_progress;
}

void Progress::resetProgress()
{
  *p_progress = 0;
}
