#ifndef event_H
#define event_H

#include <iostream>

using ipair = std::pair<int, int>;
using dpair = std::pair<double, double>;

class Event//Класс события
{

public: //Публичные методы
  Event(); //Конструктор по умолчанию

  Event(int _ev_num, double _time, int _type, ipair _sys_status, ipair _q_status,
        int  _req_num, double _time_next); //Конструктор с определением всех полей

  void set(int  _ev_num, double _time, int _type, ipair _sys_status, ipair _q_status,
           int  _req_num, double _time_next); //Метод для определения всех полей

  dpair   get_sys_status() const;
  dpair   get_q_status() const;
  double  get_time() const;

  friend std::ostream& operator<<(std::ostream&, const Event&);
  friend std::istream& operator>>(std::istream&, Event&);

public: //Публичные данные для упрощения доступа из модели
  int    ev_num;//Номер события
  double time;//Время события
  int    type;//Тип события
  ipair  sys_status;//Состояние приборов СМО
  ipair  q_status;//Состояние очереди СМО
  int    req_num;//Номер заявки
  double time_next;//Время до следующей заявки

};

#endif // event_H
