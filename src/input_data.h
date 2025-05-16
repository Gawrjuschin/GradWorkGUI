#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <cstdint>

struct InputData
{
  double mu{};
  double propability{};
  int channels{};
  int threads{};
  std::uint32_t events{};
};

#endif // INPUT_DATA_H
