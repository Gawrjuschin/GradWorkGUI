#ifndef INPUT_DATA_H
#define INPUT_DATA_H

struct InputData
{
  double mu{};
  double propability{};
  int channels{};
  int threads{};
  int events{};
  static constexpr double min_load = 0.35;
  static constexpr double max_load = 0.95;
};

#endif // INPUT_DATA_H
