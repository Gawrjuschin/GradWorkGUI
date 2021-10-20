#ifndef request_info_H
#define request_info_H

#include <iostream>

class Request_info //Класс заявки
{

public: //Методы класса
  Request_info();

  Request_info(int _type, double _arrive_time, int _queue_number, int _ch_number,
               double _start_time, double _serve_time, double _wait_time, double _serve_end);

  Request_info(const Request_info& ri);
  Request_info& operator =(const Request_info& ri);
  Request_info(Request_info&& ri) noexcept;
  Request_info& operator =(Request_info&& ri) noexcept;

  ~Request_info();

  void set(int _type, double _arrive_time, int _queue_number, int _ch_number,
           double _start_time, double _serve_time, double _wait_time, double _serve_end);


  friend std::ostream& operator <<(std::ostream &s, const Request_info& R);

  friend std::istream& operator >>(std::istream &s, Request_info& R);

public:    //Поля класса публичные для простоты заполнения
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
