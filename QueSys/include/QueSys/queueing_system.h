#ifndef queueing_system_H
#define queueing_system_H

#include "event.h"
#include "request_info.h"
#include "system_parameters_types.h"
#include <iostream>
#include <memory>
#include <vector>

using   ipair   = std::pair<int,int>;
using   dpair   = std::pair<double,double>;
using   Request = std::pair<int, Request_info>;

class Req_flow;
class Req_queue;

class Queueing_system
{

public:

  Queueing_system() = delete;

  Queueing_system(double _lambda, double _mu, int _ch_num, double _p, int _max_ev_num);

  Queueing_system(const Th_values& _th_vs, int _max_ev_num);

  ~Queueing_system( );

  void simulate();// Метод моделирования работы СМО c заданными параметрами

  void simulate(double lambda, double mu);// Метод моделирования работы СМО c изменённой интенсивностью

  void simulate(std::ostream& events_stream, std::ostream& requests_stream);
  // Метод моделирования работы СМО c заданными параметрами
  // и выводом данных в поток

  void simulate(double eps);
  // Метод моделирования работы СМО c заданной точностью

  //Константные методы доступа к полям класса
  Th_values   getTheory()        const;
  Exp_values  getExperimental()  const;
  dpair       getWait()          const;
  dpair       getUtility()       const;
  dpair       getZ()             const;
  dpair       getLambda()        const;
  dpair       getMu()            const;
  dpair       getProp()          const;
  double      getThLambda()      const;
  double      getThMu()          const;
  double      getThProp()        const;
  dpair       getThWait()        const;
  dpair       getThUtility()     const;
  int         getChNum()         const;
  double      getLoad()          const;
  ipair       s_status()         const;
  ipair       q_status()         const;
  ipair       getReqCount()      const;
  int         getReqNum()        const;
  int         getEvNum()         const;
  int         getMaxEvNum()      const;

private: // Поля класса
  Th_values   th_vs;  // Сохраняем значения параметров системы
  Exp_values  exp_vs; // Результаты симуляции работы
  std::unique_ptr<Req_flow> req_gen;
  std::unique_ptr<Req_queue>   queue;
  std::vector<Request> channels;
  ipair       status; // Количество заявок каждого типа, находящихся на обслуживании
  int         max_ev_num; // Количество генерируемых событий

  auto free_min();
};

#endif // queueing_system_H
