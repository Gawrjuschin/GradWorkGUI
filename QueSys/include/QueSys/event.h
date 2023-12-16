#ifndef event_H
#define event_H

#include <iostream>

using int_pair = std::pair<int, int>;
using double_pair = std::pair<double, double>;

class Event//Класс события
{
public: //Публичные методы
    Event() = default;

    Event(const Event&) = default;
    Event& operator=(const Event&) = default;

    Event(Event&&) = default;
    Event& operator=(Event&&) = default;

    double_pair get_sys_status() const;
    double_pair get_q_status() const;
    double get_time() const;

    friend std::ostream& operator<<(std::ostream&, const Event&);
    friend std::istream& operator>>(std::istream&, Event&);

public: //Публичные данные для упрощения доступа из модели
    int number{};             //Номер события
    double time{};            //Время события
    int type{};               //Тип события
    int_pair system_status{}; //Состояние приборов СМО
    int_pair queue_status{};  //Состояние очереди СМО
    int request{};            //Номер заявки
    double time_next{};       //Время до следующей заявки
};

#endif // event_H
