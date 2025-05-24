#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <cstdint>

/**
 * @brief The InputData class - параметры СМО из виджета InputWidget
 */
struct InputData
{
  /**
   * @brief mu - среднее время обслуживания
   */
  double mu{};
  /**
   * @brief propability - доля заявок высшего приоритета
   */
  double propability{};
  /**
   * @brief channels - количество обслуживающих приборов
   */
  int channels{};
  /**
   * @brief threads - количество потоков симуляции
   */
  int threads{};
  /**
   * @brief events - максимальное число событий симуляции
   */
  std::uint32_t events{};
};

#endif // INPUT_DATA_H
