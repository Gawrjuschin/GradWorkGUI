#include "request.h"

#include <iomanip>
#include <iostream>

#include <cstdint>

void Request::set(int _number,
		  RequestType _type,
		  double _arrive_time,
		  int _queue_number,
		  int _ch_number,
		  double _start_time,
		  double _serve_time,
		  double _wait_time,
		  double _serve_end) noexcept
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

std::ostream& operator<<(std::ostream& os, const RequestType& type)
{
  return os << static_cast<std::uint8_t>(type);
}

std::ostream& operator<<(std::ostream& os, const Request& request)
{
  return os << std::setiosflags(std::ios::fixed) << std::setprecision(6) << request.number << '\t'
	    << request.type << '\t' << request.arrive_time << '\t' << request.queue_number << '\t'
	    << request.ch_number << '\t' << request.start_time << '\t' << request.serve_time << '\t'
	    << request.wait_time << '\t' << request.serve_end << std::endl;
}

std::istream& operator>>(std::istream& is, RequestType& type)
{
  std::uint8_t type_int{};
  if (is >> type_int) {
    type = RequestType{type_int};
  } else {
    is.setstate(std::ios::badbit);
  }
  return is;
}

std::istream& operator>>(std::istream& is, Request& request)
{
  Request req{};
  if (is >> req.number && is >> req.type && is >> req.arrive_time && is >> req.queue_number
      && is >> req.ch_number && is >> req.start_time && is >> req.serve_time && is >> req.wait_time
      && is >> req.serve_end) {
    request = req;
  } else {
    is.setstate(std::ios::badbit);
  }

  return is;
}
