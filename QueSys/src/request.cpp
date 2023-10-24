#include "request.h"
#include <iomanip>

Request::Request()
    :     number{-1}, type{-1}, arrive_time{-1}, queue_number{-1}, ch_number{-1},
    start_time{-1}, serve_time{-1}, wait_time{-1}, serve_end{-1} { }

Request::Request(int _number, int _type, double _arrive_time, int _queue_number, int _ch_number,
                 double _start_time, double _serve_time, double _wait_time, double _serve_end)
    : number(_number)
    , type{_type}
    , arrive_time{_arrive_time}
    , queue_number{_queue_number}
    , ch_number{_ch_number}
    , start_time{_start_time}
    , serve_time{_serve_time}
    , wait_time{_wait_time}
    , serve_end{_serve_end}
{

}

Request::Request(const Request&) = default;
Request& Request::operator =(const Request&) = default;

Request::Request(Request&&) noexcept = default;
Request& Request::operator =(Request&&) noexcept = default;


Request::~Request() = default;

void Request::set(int _number, int _type, double _arrive_time, int _queue_number, int _ch_number,
                  double _start_time, double _serve_time, double _wait_time, double _serve_end)
{
    number = _number;
    type = _type;
    arrive_time = _arrive_time;
    queue_number = _queue_number;
    ch_number = _ch_number;
    start_time = _start_time;
    serve_time = _serve_time;
    wait_time = _wait_time;
    serve_end = _serve_end;
}

std::ostream& operator <<(std::ostream &os, const Request& request)
{
    os << std::setiosflags(std::ios::fixed) << std::setprecision(6)
      << request.number << '\t'
      << request.type << '\t'
      << request.arrive_time << '\t'
      << request.queue_number << '\t'
      << request.ch_number << '\t'
      << request.start_time << '\t'
      << request.serve_time << '\t'
      << request.wait_time << '\t'
      << request.serve_end << std::endl;
    //Спорное решение, из-за которого сложнее считывать. Пока уберу.
    //Лучше в модели это реализовать в методе data
    return os;
}

std::istream& operator >>(std::istream& is, Request& request)
{
    is >> request.number >> request.type >> request.arrive_time >> request.queue_number >> request.ch_number >> request.start_time >> request.serve_time >> request.wait_time >> request.serve_end;
    return is;
}
