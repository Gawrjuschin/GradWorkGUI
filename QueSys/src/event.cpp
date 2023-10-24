#include "event.h"
#include <iomanip>

Event::Event( )
  : ev_num{-1}, time{}, type{}, sys_status{}
  , q_status{}, req_num{-1}, time_next{}
{

}

Event::Event(int _ev_num, double _time, int _type, int_pair _sys_status, int_pair _q_status,
             int  _req_num, double _time_next)
  : ev_num{_ev_num},  time{_time},  type{_type},  sys_status{_sys_status}
  , q_status{_q_status},  req_num{_req_num},  time_next{_time_next}
{

}

double_pair   Event::get_sys_status() const
{
  return sys_status;
}

double_pair   Event::get_q_status() const
{
  return q_status;
}

double  Event::get_time() const
{
  return time;
}

void Event::set(int  _ev_num, double _time, int _type, int_pair _sys_status, int_pair _q_status,
                int  _req_num, double _time_next) //Метод для определения всех полей
{
  ev_num     = _ev_num;
  time       = _time;
  type       = _type;
  sys_status = _sys_status;
  q_status   = _q_status;
  req_num    = _req_num;
  time_next  = _time_next;
}

std::ostream& operator<<(std::ostream& s, const Event& E)
{
  constexpr int precision = 5;
  s << std::setiosflags(std::ios::fixed);
  s << std::setprecision(precision);
  s << E.ev_num << "\t" << E.time << "\t" << E.type << "\t" <<  '(' << E.sys_status.first << ", " << E.sys_status.second << "; "
    << E.q_status.first << ", " << E.q_status.second << ')' << "\t"
    << E.req_num << "\t" << E.time_next << std::endl;
  return s;
}

std::istream& operator>>(std::istream& s, Event& E)
{
  int    ev_num{0};int type{0}; int req_num{0};
  double time{0}; double time_next{0};
  int_pair  sys_status{0,0}; int_pair q_status{0,0};
  char syntax{0};
  if(   s >> std::skipws >> ev_num >> time >> type >> syntax >> sys_status.first >> syntax
        >> sys_status.second >> syntax >> q_status.first >> syntax
        >> q_status.second >> syntax >> req_num >> time_next )
    {
      E.set(ev_num,time,type,sys_status,q_status,req_num,time_next);
    }
  else
    {
      s.clear(std::ios_base::failbit);
    }
  return s;
}
