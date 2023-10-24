#include "queueing_system.h"

#include "req_flow.h"
#include "req_queue.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

Queueing_system::Queueing_system(double _lambda, double _mu, int _ch_num, double _p, int _max_ev_num)
  : th_vs(_lambda,_mu,_ch_num,_p), exp_vs{}, req_gen(new Req_flow(_lambda,_mu,_p)),
    queue(new Req_queue), channels(_ch_num), status{0,0}, max_ev_num{_max_ev_num}
{//Проверку предоставляю вызывающему
  th_vs.theory();
}
Queueing_system::Queueing_system(const Th_values& _th_vs, int _max_ev_num)
  : th_vs(_th_vs), exp_vs{}, req_gen(new Req_flow(_th_vs.L(), _th_vs.M(), _th_vs.P())),
    queue(new Req_queue), channels(_th_vs.N()), status{0,0}, max_ev_num{_max_ev_num}
{//Проверку предоставляю вызывающему
  th_vs.theory();
}

Queueing_system::~Queueing_system() = default;

Th_values  Queueing_system::getTheory()        const   {return th_vs;};
Exp_values Queueing_system::getExperimental()  const   {return exp_vs;}
double_pair      Queueing_system::getWait()          const   {return exp_vs.W();}
double_pair      Queueing_system::getUtility()       const   {return exp_vs.U();}
double_pair      Queueing_system::getZ()             const   {return exp_vs.Z();}
double_pair      Queueing_system::getLambda()        const   {return exp_vs.L();}
double_pair      Queueing_system::getMu()            const   {return exp_vs.M();}
double_pair      Queueing_system::getProp()          const   {return exp_vs.P();}
double     Queueing_system::getThLambda()      const   {return th_vs.L();}
double     Queueing_system::getThMu()          const   {return th_vs.M();}
double     Queueing_system::getThProp()        const   {return th_vs.P();}
double_pair      Queueing_system::getThWait()        const   {return th_vs.W();}
double_pair      Queueing_system::getThUtility()     const   {return th_vs.U();}
int        Queueing_system::getChNum()         const   {return th_vs.N();}
double     Queueing_system::getLoad()          const   {return th_vs.Load();}
int_pair      Queueing_system::s_status()         const   {return status; }
int_pair      Queueing_system::q_status()         const   {return queue->status();}
int_pair      Queueing_system::getReqCount()      const   {return exp_vs.Req_count();}
// Заявок генерируется на 1 больше, а счётчик событий спешит на 1
int        Queueing_system::getReqNum()        const   {return exp_vs.Req_num() - 1;}
int        Queueing_system::getEvNum()         const   {return exp_vs.Ev_num() - 1;}
int        Queueing_system::getMaxEvNum()      const   {return max_ev_num;}

auto Queueing_system::free_min()
{
  auto free = channels.end();
  auto min = channels.begin();
  for(auto iter = channels.begin(); iter!=channels.end(); ++iter)
    {
      min = (iter->serve_end != -1) && (iter->serve_end < min->serve_end || min->serve_end == -1)
          ? iter
          : min;
      free = (free == channels.end() && iter->serve_end == -1)
          ? iter
          : free;
    }
  return make_pair(free,min);
}

void Queueing_system::simulate(double lambda, double mu)//Метод моделирования работы СМО c изменённой интенсивностью
{ // Метод, запускающий симуляцию с другими параметрами интенсивности
  th_vs.rL() = lambda;
  th_vs.rM() = mu;
  th_vs.theory();
  queue->clear();
  std::unique_ptr<Req_flow> ptr(req_gen.release());
  req_gen.reset(new Req_flow(th_vs.L(), th_vs.M(), th_vs.P()));
  simulate();
}

void Queueing_system::simulate()//Нужно установить условие сходимости и добавить такую опцию в виджет
{ //Симуляция для первых max_ev_num событий
  //Сброс системы (эксп. значения и каналы)
  Request free_channel{0, -1,-1,-1,-1,-1,-1,-1,-1};
  for(auto& ch : channels)//Инициализируем каналы
    {
      ch = free_channel;
    }
  exp_vs = {}; //Сброс теоретических значений
  status = {0,0};
  queue->clear();//Сброс очереди
  //Ссылки как сокращения
  auto& evc   = exp_vs.Ev_num();//В систему поступила первая заявка
  auto& request_number  = exp_vs.Req_num();//Счётчик поступивших заявок
  auto& time  = exp_vs.T();//Замены (для краткости)
  auto& r     = exp_vs.Req_count();//Счётчик обработанных заявок по приоритетам
  auto& mu    = exp_vs.Mu_sum();
  auto& w     = exp_vs.W_sum();
  auto& wc    = exp_vs.W_count();
  auto& z     = exp_vs.Z_sum();
  Req_flow& req_generator = *req_gen;
  Request Next_Req = req_generator();//Следующая заявка
  Event Current_Event;//Текущее событие. Постоянно меняется через метод set(...)
  double prev_time{Next_Req.arrive_time}; //Время предыдущего события
  int_pair prev_z{0,0};  //Количество заявок до события для подсчёта среднего арифметического
  while(evc <= max_ev_num)
    {
      auto [free, min] = free_min();
          auto branch = (Next_Req.arrive_time < min->serve_end || min->serve_end == -1)
          //Попытка встать на обслуживание
          ? ( (free != channels.end()) ? ( (!queue->empty()) ? 2 : 1 ) : 4 )
          // Есть свободный прибор "?" на обслуживание( очередь пуста "?" пришедшую "1" ":" первую в очереди "2")
          // все заняты ":" в очередь "4"
          : ( ((free != channels.end()) && !queue->empty()) ?  2 : 3);
          // Есть свободный прибор и заявка в очереди "?" на обслуживание первую в очереди "2" ":" освободить ближайшую заявку "3"
          switch (branch)
      {
          case 1://Приборы простаивают, очередь пустая
            {    //Пришедшая заявка встаёт на обслуживание
              *free = Next_Req; //Первый свободный прибор принимает заявку
              Next_Req = req_generator();
              time = free->arrive_time;
              free->queue_number = 0;
              free->ch_number    = free - channels.begin() + 1;//Лучше distance
              free->start_time   = free->arrive_time;
              free->serve_end    = free->arrive_time + free->serve_time;
              free->wait_time    = 0;
              (free->type == 1) ? status.first++ : status.second++;// Изменение количества заявок в СМО
              Current_Event.set(evc, time, (free->type == 1) ? 1 : 2, status, queue->status(),
                                free->number, Next_Req.arrive_time - free->arrive_time);
              break;
            }//case 1
          case 2://Приборы простаивают, очередь не пустая
            {    //Первая в очереди заявка встаёт на обслуживание
              //Нужно проверить случай, когда Next_Request.second.time == min->serve_end
              (*free) = queue->front();
              (free->type == 1) ? status.first++ : status.second++;// Изменение количества заявок в СМО
              queue->pop();
              free->ch_number    = free - channels.begin() + 1;//Лучше distance
              free->start_time   = time;
              free->wait_time    = time - free->arrive_time;
              free->serve_end    = time + free->serve_time;
              Current_Event.set(evc, time, (free->type == 1) ? 7 : 8, status, queue->status(),
                                free->number, Next_Req.arrive_time - time);//Запись данных в новое событие
              break;
            }//case 2
          case 3://Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            {    //Освобождается прибор с минимальным оставшимся временем обслуживания
              time = min->serve_end;// Запоминается время выхода заявки. См. case 2:
              (min->type == 1) ? status.first-- : status.second--;// Изменение количества заявок в СМО
              Current_Event.set(evc, time, (min->type == 1) ? 5 : 6, status, queue->status(),
                                min->number, Next_Req.arrive_time - time);
              if (min->type == 2) //(*min) - освободившаяся заявка
                {//Только здесь изменяется число заявок в системе
                  mu.second += min->serve_time;
                  r.second++;
                  ((min->wait_time > 0) ? (w.second += min->wait_time, wc.second++) : 0);
                }
              else
                {
                  mu.first += min->serve_time;
                  r.first++;
                  ((min->wait_time > 0) ? (w.first += min->wait_time, wc.first++) : 0);
                }
              (*min) = free_channel;// Освобождение прибора с минимальным оставшимся временем обслуживания
              break;
            }//case 3
          case 4://Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            {//Следующая заявка встаёт в очередь. Место в очереди вычисляется перед отправкой в неё
              double prev_arrive_time = Next_Req.arrive_time;
              Next_Req.queue_number   = queue->status().first + (queue->status().second * (Next_Req.type - 1)) + 1;
              queue->push(Next_Req);
              time = Next_Req.arrive_time;
              Next_Req = req_generator();//Вместо *min = Next_Req++;
              Current_Event.set(evc, time, (Next_Req.type == 1) ? 3 : 4, status, queue->status(),
                                (Next_Req.number - 1), Next_Req.arrive_time - prev_arrive_time);
                                /*Номер пред.зааявки на 1 меньше*/
              break;
            }//case 4
        }//switch
      //Полностью заменяет проход по событиям из оригинальной работы
      z.first   += (Current_Event.get_time() - prev_time) * prev_z.first;//Теперь учитывается проведённое в состоянии время
      z.second  += (Current_Event.get_time() - prev_time) * prev_z.second;
      prev_time = Current_Event.get_time(); //Запоминается время события
      prev_z    = {Current_Event.get_q_status().first  + Current_Event.get_sys_status().first,//Запоминаем состояние
                   Current_Event.get_q_status().second + Current_Event.get_sys_status().second};
      evc++;
    }//while
  exp_vs.experimental();
  return;
}

void Queueing_system::simulate(std::ostream& events_stream, std::ostream& requests_stream)
{ //Симуляция для первых 1000 событий
  max_ev_num = 1000;

  events_stream << "Первые " << max_ev_num << " событий системы:\n";
  events_stream << "№\ttime\ttype\t(sys; que)\treq_num\ttime_next\n";
  requests_stream << "Обработанные заявки за первые " << max_ev_num << " событий\n";
  requests_stream << "№\ttype\tarrive\t\tqnum\tchnum\tstart\t\tserve\t\twait\t\tend\n";
  requests_stream << std::setiosflags(std::ios::fixed);
  requests_stream << std::setprecision(5);
  //Сброс системы (эксп. значения и каналы)
  Request free_channel{0,-1,-1,-1,-1,-1,-1,-1,-1};
  for(auto& ch : channels)//Инициализируем каналы
    {
      ch = free_channel;
    }
  exp_vs = {}; //Сброс теоретических значений
  status = {0,0};
  queue->clear();//Сброс очереди
  //Ссылки как сокращения
  auto& evc   = exp_vs.Ev_num();//В систему поступила первая заявка
  auto& reqc  = exp_vs.Req_num();//Счётчик поступивших заявок
  auto& time  = exp_vs.T();//Замены (для краткости)
  auto& r     = exp_vs.Req_count();//Счётчик обработанных заявок по приоритетам
  auto& mu    = exp_vs.Mu_sum();
  auto& w     = exp_vs.W_sum();
  auto& wc    = exp_vs.W_count();
  auto& z     = exp_vs.Z_sum();
  Req_flow& gen = *req_gen;
  Request Next_Req = gen();//Следующая заявка
  Event Current_Event;//Текущее событие. Постоянно меняется через метод set(...)
  double prev_time{Next_Req.arrive_time}; //Время предыдущего события
  int_pair prev_z{0,0};  //Количество заявок до события для подсчёта среднего арифметического
  while(evc <= max_ev_num)
    {
      auto [free, min] = free_min();
          auto branch = (Next_Req.arrive_time < min->serve_end || min->serve_end == -1)
          //Попытка встать на обслуживание
          ? ( (free != channels.end()) ? ( (!queue->empty()) ? 2 : 1 ) : 4 )
          // Есть свободный прибор "?" на обслуживание( очередь пуста "?" пришедшую "1" ":" первую в очереди "2")
          // все заняты ":" в очередь "4"
          : ( ((free != channels.end()) && !queue->empty()) ?  2 : 3);
          // Есть свободный прибор и заявка в очереди "?" на обслуживание первую в очереди "2" ":" освободить ближайшую заявку "3"
          switch (branch)
      {
          case 1://Приборы простаивают, очередь пустая
            {    //Пришедшая заявка встаёт на обслуживание
              *free = Next_Req; //Первый свободный прибор принимает заявку
              Next_Req = gen();
              time = free->arrive_time;
              free->queue_number = 0;
              free->ch_number    = free - channels.begin() + 1;//Лучше distance
              free->start_time   = free->arrive_time;
              free->serve_end    = free->arrive_time + free->serve_time;
              free->wait_time    = 0;
              (free->type == 1) ? status.first++ : status.second++;// Изменение количества заявок в СМО
              Current_Event.set(evc, time, (free->type == 1) ? 1 : 2, status, queue->status(),
                                free->number, Next_Req.arrive_time - free->arrive_time);
              break;
            }//case 1
          case 2://Приборы простаивают, очередь не пустая
            {    //Первая в очереди заявка встаёт на обслуживание
              //Нужно проверить случай, когда Next_Request.second.time == min->serve_end
              (*free) = queue->front();
              (free->type == 1) ? status.first++ : status.second++;// Изменение количества заявок в СМО
              queue->pop();
              free->ch_number    = free - channels.begin() + 1;//Лучше distance
              free->start_time   = time;
              free->wait_time    = time - free->arrive_time;
              free->serve_end    = time + free->serve_time;
              Current_Event.set(evc, time, (free->type == 1) ? 7 : 8, status, queue->status(),
                                free->number, Next_Req.arrive_time - time);//Запись данных в новое событие
              break;
            }//case 2
          case 3://Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            {    //Освобождается прибор с минимальным оставшимся временем обслуживания
              time = min->serve_end;// Запоминается время выхода заявки. См. case 2:
              (min->type == 1) ? status.first-- : status.second--;// Изменение количества заявок в СМО
              Current_Event.set(evc, time, (min->type == 1) ? 5 : 6, status, queue->status(),
                                min->number, Next_Req.arrive_time - time);
              if (min->type == 2) //(*min) - освободившаяся заявка
                {//Только здесь изменяется число заявок в системе
                  mu.second += min->serve_time;
                  r.second++;
                  ((min->wait_time > 0) ? (w.second += min->wait_time, wc.second++) : 0);
                }
              else
                {
                  mu.first += min->serve_time;
                  r.first++;
                  ((min->wait_time > 0) ? (w.first += min->wait_time, wc.first++) : 0);
                }
                requests_stream << *min;
              (*min) = free_channel;// Освобождение прибора с минимальным оставшимся временем обслуживания
              break;
            }//case 3
          case 4://Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            {//Следующая заявка встаёт в очередь. Место в очереди вычисляется перед отправкой в неё
              double prev_arrive_time = Next_Req.arrive_time;
              Next_Req.queue_number   = queue->status().first + (queue->status().second * (Next_Req.type - 1)) + 1;
              queue->push(Next_Req);
              time = Next_Req.arrive_time;
              Next_Req = gen();//Вместо *min = Next_Req++;
              Current_Event.set(evc, time, (Next_Req.type == 1) ? 3 : 4, status, queue->status(),
                                (Next_Req.number - 1), Next_Req.arrive_time - prev_arrive_time);
                                /*Номер пред.зааявки на 1 меньше*/
              break;
            }//case 4
        }//switch
      z.first   += (Current_Event.get_time() - prev_time) * prev_z.first;//Теперь учитывается проведённое в состоянии время
      z.second  += (Current_Event.get_time() - prev_time) * prev_z.second;
      prev_time = Current_Event.get_time(); //Запоминается время события
      prev_z    = {Current_Event.get_q_status().first  + Current_Event.get_sys_status().first,//Запоминаем состояние
                   Current_Event.get_q_status().second + Current_Event.get_sys_status().second};
      events_stream << Current_Event;
      evc++;
    }//while
  for(auto& req : channels)
    {
      if(req.number!= 0)
        {
          requests_stream << req;
        }
    }
  if(queue->empty())
    {
      requests_stream << "\nОчередь пуста!\n";
    }
  else
    { // Записываем заявки в файл, поочерёдно вытаскивая из очереди, а потом возвращаем их обратно
      std::vector<Request> vec;
      vec.reserve(queue->status().first + queue->status().second);
      requests_stream << "\nЗаявки, оставшиеся в очереди:\n";
      while(!queue->empty())
        {
          requests_stream << queue->front().number << '\t' << queue->front().number;
          vec.push_back(queue->front());
          queue->pop();
        }
      for(auto& x : vec)
        {
          queue->push(x);
        }
    } //Очередь опустошается!
  exp_vs.experimental();
}

void Queueing_system::simulate(double eps)//Нужно установить условие сходимости и добавить такую опцию в виджет
{ //Симуляция до сходимости по вероятности
  //Сброс системы (эксп. значения и каналы)
  Request free_channel{0, -1,-1,-1,-1,-1,-1,-1,-1};
  for(auto& ch : channels)//Инициализируем каналы
    {
      ch = free_channel;
    }
  exp_vs = {}; //Сброс теоретических значений
  status = {0,0};
  queue->clear();//Сброс очереди
  double_pair delta{1,2}; //Дельта для подсчёта с заданной точностью
  //Ссылки как сокращения
  auto& evc   = exp_vs.Ev_num();//В систему поступила первая заявка
  auto& reqc  = exp_vs.Req_num();//Счётчик поступивших заявок
  auto& time  = exp_vs.T();//Замены (для краткости)
  auto& r     = exp_vs.Req_count();//Счётчик обработанных заявок по приоритетам
  auto& mu    = exp_vs.Mu_sum();
  auto& w     = exp_vs.W_sum();
  auto& wc    = exp_vs.W_count();
  auto& z     = exp_vs.Z_sum();
  Req_flow& gen = *req_gen;
  Request Next_Req = gen();//Следующая заявка
  Event Current_Event;//Текущее событие. Постоянно меняется через метод set(...)
  double prev_time{Next_Req.arrive_time}; //Время предыдущего события
  int_pair prev_z{0,0};  //Количество заявок до события для подсчёта среднего арифметического
  while(delta.second > eps)
    {
      auto [free, min] = free_min();
          auto branch = (Next_Req.arrive_time < min->serve_end || min->serve_end == -1)
          //Попытка встать на обслуживание
          ? ( (free != channels.end()) ? ( (!queue->empty()) ? 2 : 1 ) : 4 )
          // Есть свободный прибор "?" на обслуживание( очередь пуста "?" пришедшую "1" ":" первую в очереди "2")
          // все заняты ":" в очередь "4"
          : ( ((free != channels.end()) && !queue->empty()) ?  2 : 3);
          // Есть свободный прибор и заявка в очереди "?" на обслуживание первую в очереди "2" ":" освободить ближайшую заявку "3"
          switch (branch)
      {
          case 1://Приборы простаивают, очередь пустая
            {    //Пришедшая заявка встаёт на обслуживание
              *free = Next_Req; //Первый свободный прибор принимает заявку
              Next_Req = gen();
              time = free->arrive_time;
              free->queue_number = 0;
              free->ch_number    = free - channels.begin() + 1;//Лучше distance
              free->start_time   = free->arrive_time;
              free->serve_end    = free->arrive_time + free->serve_time;
              free->wait_time    = 0;
              (free->type == 1) ? status.first++ : status.second++;// Изменение количества заявок в СМО
              Current_Event.set(evc, time, (free->type == 1) ? 1 : 2, status, queue->status(),
                                free->number, Next_Req.arrive_time - free->arrive_time);
              break;
            }//case 1
          case 2://Приборы простаивают, очередь не пустая
            {    //Первая в очереди заявка встаёт на обслуживание
              //Нужно проверить случай, когда Next_Request.second.time == min->serve_end
              (*free) = queue->front();
              (free->type == 1) ? status.first++ : status.second++;// Изменение количества заявок в СМО
              queue->pop();
              free->ch_number    = free - channels.begin() + 1;//Лучше distance
              free->start_time   = time;
              free->wait_time    = time - free->arrive_time;
              free->serve_end    = time + free->serve_time;
              Current_Event.set(evc, time, (free->type == 1) ? 7 : 8, status, queue->status(),
                                free->number, Next_Req.arrive_time - time);//Запись данных в новое событие
              break;
            }//case 2
          case 3://Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            {    //Освобождается прибор с минимальным оставшимся временем обслуживания
              time = min->serve_end;// Запоминается время выхода заявки. См. case 2:
              (min->type == 1) ? status.first-- : status.second--;// Изменение количества заявок в СМО
              Current_Event.set(evc, time, (min->type == 1) ? 5 : 6, status, queue->status(),
                                min->number, Next_Req.arrive_time - time);
              if (min->type == 2) //(*min) - освободившаяся заявка
                {//Только здесь изменяется число заявок в системе
                  mu.second += min->serve_time;
                  r.second++;
                  ((min->wait_time > 0) ? (w.second += min->wait_time, wc.second++) : 0);
                }
              else
                {
                  mu.first += min->serve_time;
                  r.first++;
                  ((min->wait_time > 0) ? (w.first += min->wait_time, wc.first++) : 0);
                }
              auto next_delta = std::abs(delta.first - (double(r.first) /
                                                        (r.first + r.second) ));
              if(next_delta < delta.second && next_delta != 0)
                { //Это минимальная дельта
                  delta.second = next_delta;
                }
              delta.first    = double(r.first) /      //Вычисляем текущее значение приближения С.В.
                  (r.first + r.second);
              (*min) = free_channel;// Освобождение прибора с минимальным оставшимся временем обслуживания
              break;
            }//case 3
          case 4://Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            {//Следующая заявка встаёт в очередь. Место в очереди вычисляется перед отправкой в неё
              double prev_arrive_time = Next_Req.arrive_time;
              Next_Req.queue_number   = queue->status().first + (queue->status().second * (Next_Req.type - 1)) + 1;
              queue->push(Next_Req);
              time = Next_Req.arrive_time;
              Next_Req = gen();//Вместо *min = Next_Req++;
              Current_Event.set(evc, time, (Next_Req.type == 1) ? 3 : 4, status, queue->status(),
                                (Next_Req.number - 1), Next_Req.arrive_time - prev_arrive_time);
                                /*Номер пред.заявки на 1 меньше*/
              break;
            }//case 4
        }//switch
      z.first   += (Current_Event.get_time() - prev_time) * prev_z.first;//Теперь учитывается проведённое в состоянии время
      z.second  += (Current_Event.get_time() - prev_time) * prev_z.second;
      prev_time = Current_Event.get_time(); //Запоминается время события
      prev_z    = {Current_Event.get_q_status().first  + Current_Event.get_sys_status().first,//Запоминаем состояние
                   Current_Event.get_q_status().second + Current_Event.get_sys_status().second};
      evc++;
    }//while
  exp_vs.experimental();
}
