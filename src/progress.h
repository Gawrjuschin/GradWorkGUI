#ifndef PROGRESS_H
#define PROGRESS_H

#include <memory>

class QAtomicInt;

class Progress
{

public:
  Progress( );
  ~Progress( );

  int  operator ++( );
  int  operator ++(int);

  int  getProgress();
  void resetProgress();

private:
  std::unique_ptr<QAtomicInt> p_progress;

};

#endif // PROGRESS_H
