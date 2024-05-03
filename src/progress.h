#ifndef PROGRESS_H
#define PROGRESS_H

#include <atomic>
#include <cstdint>

class Progress {
public:
  Progress() = default;

  Progress(const Progress&) = delete;
  Progress& operator=(const Progress&) = delete;

  Progress(Progress&&) = delete;
  Progress& operator=(Progress&&) = delete;

  ~Progress() = default;

  operator std::uint32_t() const noexcept {
    return m_value.load(std::memory_order_acquire);
  }
  std::uint32_t value() const noexcept {
    return m_value.load(std::memory_order_acquire);
  }

  std::uint32_t arrive() noexcept { return ++m_value; };
  void reset() noexcept { m_value.store(0, std::memory_order_release); }

private:
  std::atomic_uint32_t m_value{};
};

#endif // PROGRESS_H
