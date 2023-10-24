#ifndef event_H
#define event_H

#include <iostream>

using int_pair = std::pair<int, int>;
using double_pair = std::pair<double, double>;

class Event//Класс события
{

public: //Публичные методы
  Event(); //Конструктор по умолчанию

  Event(int _ev_num, double _time, int _type, int_pair _sys_status, int_pair _q_status,
        int  _req_num, double _time_next); //Конструктор с определением всех полей

  void set(int  _ev_num, double _time, int _type, int_pair _sys_status, int_pair _q_status,
           int  _req_num, double _time_next); //Метод для определения всех полей

  double_pair   get_sys_status() const;
  double_pair   get_q_status() const;
  double  get_time() const;

  friend std::ostream& operator<<(std::ostream&, const Event&);
  friend std::istream& operator>>(std::istream&, Event&);

public: //Публичные данные для упрощения доступа из модели
  int    ev_num;//Номер события
  double time;//Время события
  int    type;//Тип события
  int_pair  sys_status;//Состояние приборов СМО
  int_pair  q_status;//Состояние очереди СМО
  int    req_num;//Номер заявки
  double time_next;//Время до следующей заявки

};

#endif // event_H
