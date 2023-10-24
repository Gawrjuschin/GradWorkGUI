#include <QueSys/queueing_system.h>

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_map>

using double_pair = std::pair<double, double>;

constexpr int max_ev = 1000;
constexpr double eps = 0.001;

void tester()
{
  auto output = std::ofstream("debug.txt");
  output << std::setiosflags(std::ios::fixed);
  output << std::setprecision(6);
  constexpr double dl = 0.1;
  constexpr double m = 1;
  constexpr int n = 5;
  constexpr double p = 0.25;
  constexpr double load = 0.35; // Начальная нагрузка
  double l = n * m * load;
  for(; l / m / n < 0.95; l+=dl)
    {
      Queueing_system sys(l, m, n, p, max_ev);//Изменяем мю
      sys.simulate();
      output << "Load: " << l/m/n << "\t(" << l << ", " << m << ")\t("
             << sys.getZ().first << ", " << sys.getZ().second << ")\t("
             << sys.getWait().first << ", " << sys.getWait().second << ")\n";
    }
  constexpr double _m = 2;
  l = n * _m * load;
    for(; l / _m / n < 0.95; l+=dl)
      {//Проверка зависимости от нагрузки
        Queueing_system sys(l, _m, n, p, max_ev);//Изменяем мю
        sys.simulate();
        output << "Load: " << l/_m/n << "\t(" << l << ", " << _m << ")\t("
               << sys.getZ().first << ", " << sys.getZ().second << ")\t("
             << sys.getWait().first << ", " << sys.getWait().second << ")\n";
      }
    {//Проверка вывода в поток
      auto load = 0.8;
      l = n * _m * load;
      Queueing_system sys(l, _m, n, p, 100);//Изменяем мю
      auto evstream = std::ofstream("evs.txt");
      output << std::setiosflags(std::ios::fixed);
      output << std::setprecision(6);
      auto reqstream = std::ofstream("reqs.txt");
      output << std::setiosflags(std::ios::fixed);
      output << std::setprecision(6);
      sys.simulate(evstream,reqstream);
    }
    {//Проверка сходимости
      l = n * _m * load;
      Queueing_system sys(l, _m, n, p, max_ev);//Изменяем мю
      sys.simulate(eps);
    }
}

int main()
{
  auto start = std::chrono::system_clock::now();
  try
  {
    tester();
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  catch(std::string& e)
  {
    std::cerr << e << '\n';
    return 2;
  }
  catch(const char* e)
  {
    std::cerr << e << '\n';
    return 3;
  }
  catch(...)
  {
    std::cerr << "Oops: unknown error!\n";
    return 4;
  }
  auto end = std::chrono::system_clock::now();
  auto elapced = end - start;
  std::cout << "Completed in " << (elapced.count() > 1000 ? std::to_string(std::chrono::duration_cast<std::chrono::seconds>(elapced).count()) + "s and " : "")
       << (std::chrono::duration_cast<std::chrono::milliseconds>(elapced).count() - 1000 * std::chrono::duration_cast<std::chrono::seconds>(elapced).count())
       << "ms\n";
  system("pause");
  return 0;
}
