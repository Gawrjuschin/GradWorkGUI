#include "request_info.h"
#include <iomanip>

Request_info::Request_info()
  :     type{-1}, arrive_time{-1}, queue_number{-1}, ch_number{-1},
    start_time{-1}, serve_time{-1}, wait_time{-1}, serve_end{-1} { }

Request_info::Request_info(int _type, double _arrive_time, int _queue_number, int _ch_number,
                           double _start_time, double _serve_time, double _wait_time, double _serve_end)
  : type{_type}
  , arrive_time{_arrive_time}
  , queue_number{_queue_number}
  , ch_number{_ch_number}
  , start_time{_start_time}
  , serve_time{_serve_time}
  , wait_time{_wait_time}
  , serve_end{_serve_end}
{

}

Request_info::Request_info(const Request_info&) = default;
Request_info& Request_info::operator =(const Request_info&) = default;
Request_info::Request_info(Request_info&&) noexcept = default;
Request_info& Request_info::operator =(Request_info&&) noexcept = default;


Request_info::~Request_info() = default;

void Request_info::set(int _type, double _arrive_time, int _queue_number, int _ch_number,
                       double _start_time, double _serve_time, double _wait_time, double _serve_end)
{
  type = _type;
  arrive_time = _arrive_time;
  queue_number = _queue_number;
  ch_number = _ch_number;
  start_time = _start_time;
  serve_time = _serve_time;
  wait_time = _wait_time;
  serve_end = _serve_end;
}

std::ostream& operator <<(std::ostream &s, const Request_info& R)
{
  s << std::setiosflags(std::ios::fixed) << std::setprecision(6)
    << R.type << '\t' << R.arrive_time << '\t' << R.queue_number << '\t'
    << R.ch_number << '\t' << R.start_time << '\t' << R.serve_time << '\t'
    << R.wait_time << '\t' << R.serve_end <<std::endl;
  //Спорное решение, из-за которого сложнее считывать. Пока уберу.
  //Лучше в модели это реализовать в методе data
  return s;
}

std::istream& operator >>(std::istream &s, Request_info& R)
{
  int type{0}; //Тип заявки (1 или 2)
  double arrive_time{0}; //Время прибытия
  int queue_number{0}; //Номер в очереди
  int ch_number{0}; //Номер прибора, обслуживающего заявку
  double start_time{0}; //Время начала обслуживания
  double serve_time{0}; //Время обслуживания
  double wait_time{0}; //Время ожидания в очереди
  double serve_end{0}; //Время окончания обслуживания
  if(s >> std::skipws >> type >> arrive_time >> queue_number >> ch_number >> start_time >> serve_time >> wait_time >> serve_end)
    {
      R.set(type, arrive_time, queue_number, ch_number,
            start_time, serve_time, wait_time, serve_end);
    }
  else
    {
      s.clear(std::ios_base::failbit);
    }
  return s;
}
