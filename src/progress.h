#ifndef PROGRESS_H
#define PROGRESS_H

#include <atomic>
#include <cstdint>

/**
 * @brief The Progress class - посредник для учёта прогресса в выполнении
 * симуляции. Требуется вызывать arrive после каждого выполненного чанка
 * симуляции.
 */
class Progress {
public:
  Progress() = default;

  Progress(const Progress&) = delete;
  Progress& operator=(const Progress&) = delete;

  Progress(Progress&&) = delete;
  Progress& operator=(Progress&&) = delete;

  ~Progress() = default;

  /**
   * @brief operator std::uint32_t - неявное преобразование к uint32_t
   */
  operator std::uint32_t() const noexcept {
    return m_value.load(std::memory_order_acquire);
  }

  /**
   * @brief value - метод явного преобразования к uint32_t
   * @return
   */
  std::uint32_t value() const noexcept {
    return m_value.load(std::memory_order_acquire);
  }

  /**
   * @brief arrive - метод, который требуется вызывать по выполнению чанка
   * симуляции
   * @return
   */
  std::uint32_t arrive() noexcept { return ++m_value; };

  /**
   * @brief reset - сброс прогресса симуляции
   */
  void reset() noexcept { m_value.store(0, std::memory_order_release); }

private:
  std::atomic_uint32_t m_value{};
};

#endif // PROGRESS_H
