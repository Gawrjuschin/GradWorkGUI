#ifndef request_info_H
#define request_info_H

#include <iostream>

class Request //Класс заявки
{

public: //Методы класса
  Request();

  Request(int _number, int _type, double _arrive_time, int _queue_number, int _ch_number,
               double _start_time, double _serve_time, double _wait_time, double _serve_end);

  Request(const Request& ri);
  Request& operator =(const Request& ri);
  Request(Request&& ri) noexcept;
  Request& operator =(Request&& ri) noexcept;

  ~Request();

  void set(int _number, int _type, double _arrive_time, int _queue_number, int _ch_number,
           double _start_time, double _serve_time, double _wait_time, double _serve_end);


  friend std::ostream& operator <<(std::ostream& os, const Request& request);
  friend std::istream& operator >>(std::istream& is, Request& request);

public:    //Поля класса публичные для простоты заполнения
  int number; // Номер заявки
  int type; //Тип заявки (1 или 2)
  double arrive_time; //Время прибытия
  int queue_number; //Номер в очереди
  int ch_number; //Номер прибора, обслуживающего заявку
  double start_time; //Время начала обслуживания
  double serve_time; //Время обслуживания
  double wait_time; //Время ожидания в очереди
  double serve_end; //Время окончания обслуживания

};

#endif // request_info_H
