#ifndef PROGRESS_H
#define PROGRESS_H

#include <memory>

#include <QAtomicInt>

class Progress
{
public:
  Progress() = default;

  Progress(const Progress&) = delete;
  Progress& operator=(const Progress&) = delete;

  Progress(Progress&&) = delete;
  Progress& operator=(Progress&&) = delete;

  ~Progress();

  int operator++();
  int operator++(int);

  int value() const noexcept;

  void resetProgress();

private:
  QAtomicInt m_progress{};
};

#endif // PROGRESS_H
