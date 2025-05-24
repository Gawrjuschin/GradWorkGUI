#ifndef THREADS_CONTROL_H
#define THREADS_CONTROL_H

#include <atomic>
#include <condition_variable>
#include <mutex>

/**
 * @brief The ThreadsControl class - отвечает за управление чанками симуляции.
 * Треует вызова ThreadsControl::sleep() перед полезной нагрузкой для поддержки
 * паузы.
 */
class ThreadsControl {
  std::atomic_flag m_pause_flag{};
  std::atomic_flag m_cancel_flag{};
  mutable std::mutex m_mutex{};
  mutable std::condition_variable m_cvar{};

public:
  ThreadsControl() = default;

  ThreadsControl(const ThreadsControl&) = delete;
  ThreadsControl& operator=(const ThreadsControl&) = delete;

  ThreadsControl(ThreadsControl&&) = delete;
  ThreadsControl& operator=(ThreadsControl&&) = delete;

  ~ThreadsControl() = default;

  /**
   * @brief sleep - реализует поддержку паузы и возобновления
   */
  void sleep() const {
    if (paused()) {
      std::unique_lock lck(m_mutex);
      m_cvar.wait(lck, [this] { return !paused(); });
    }
  }

  /**
   * @brief pause - постановка процесса симуляции на паузу в месте вызова
   * ThreadsControl::sleep
   */
  void pause() noexcept {
    m_pause_flag.test_and_set(std::memory_order_release);
  }

  /**
   * @brief resume - возобновление процесса симуляции в месте вызова
   */
  void resume() noexcept {
    m_pause_flag.clear(std::memory_order_release);
    m_cvar.notify_all();
  }

  /**
   * @brief cancel - остановить процесс симуляции.
   */
  void cancel() noexcept {
    m_cancel_flag.test_and_set(std::memory_order_release);
    if (paused()) {
      resume();
    }
  }
  /**
   * @brief reset - сброс флагов.
   */
  void reset() noexcept {
    m_cancel_flag.clear(std::memory_order_release);
    m_pause_flag.clear(std::memory_order_release);
  }
  /**
   * @brief paused - значение флага паузы.
   * @return
   */
  bool paused() const noexcept {
    return m_pause_flag.test(std::memory_order_acquire);
  }
  /**
   * @brief canceled - значение флага отмены. Требуется проверять на каждой
   * итерации внутри чанка симуляции.
   * @return
   */
  bool canceled() const noexcept {
    return m_cancel_flag.test(std::memory_order_acquire);
  }
};

#endif // THREADS_CONTROL_H
