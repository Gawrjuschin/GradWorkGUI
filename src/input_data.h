#ifndef INPUT_DATA_H
#define INPUT_DATA_H

struct InputData
{
  double mu{};
  double propability{};
  int channels{};
  int threads{};
  int events{};
  unsigned int seed{};
};

#endif // INPUT_DATA_H
