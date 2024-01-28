#ifndef THREADSCONTROL_H
#define THREADSCONTROL_H

#include <atomic>
#include <condition_variable>
#include <mutex>

class Progress
{
public:
  Progress() = default;

  Progress(const Progress&) = delete;
  Progress& operator=(const Progress&) = delete;

  Progress(Progress&&) = delete;
  Progress& operator=(Progress&&) = delete;

  ~Progress() = default;

  operator std::uint32_t() const noexcept { return m_value.load(std::memory_order_acquire); }
  std::uint32_t value() const noexcept { return m_value.load(std::memory_order_acquire); }

  std::uint32_t arrive() noexcept { return ++m_value; };
  void reset() noexcept { m_value.store(0, std::memory_order_release); }

private:
  std::atomic_uint32_t m_value{};
};

class ThreadsControl
{
public:
  ThreadsControl() = default;

  ThreadsControl(const ThreadsControl&) = delete;
  ThreadsControl& operator=(const ThreadsControl&) = delete;

  ThreadsControl(ThreadsControl&&) = delete;
  ThreadsControl& operator=(ThreadsControl&&) = delete;

  ~ThreadsControl() = default;

  void sleep() const
  {
    if (paused()) {
      std::unique_lock lck(m_mutex);
      m_cvar.wait(lck, [this] { return !paused(); });
    }
  }

  void pause() noexcept { m_pause_flag.test_and_set(std::memory_order_release); }
  void resume() noexcept { m_pause_flag.clear(std::memory_order_release); }

  void cancel() noexcept
  {
    m_cancel_flag.test_and_set(std::memory_order_release);
    if (paused()) {
      m_cvar.notify_all();
    }
  }
  void reset() noexcept
  {
    m_cancel_flag.clear(std::memory_order_release);
    m_pause_flag.clear(std::memory_order_release);
  }

  bool paused() const noexcept { return m_pause_flag.test(std::memory_order_acquire); }
  bool canceled() const noexcept { return m_cancel_flag.test(std::memory_order_acquire); }

private:
  std::atomic_flag m_pause_flag{};
  std::atomic_flag m_cancel_flag{};
  mutable std::mutex m_mutex{};
  mutable std::condition_variable m_cvar{};
};

#endif // THREADSCONTROL_H
