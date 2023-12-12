#ifndef queueing_system_H
#define queueing_system_H

#include "event.h"
#include "request.h"
#include "system_parameters_types.h"
#include <iostream>
#include <memory>
#include <vector>

using int_pair = std::pair<int, int>;
using double_pair = std::pair<double, double>;

class Req_flow;
class Req_queue;

class Queueing_system
{
public:
    Queueing_system() = delete;

    Queueing_system(double _lambda, double _mu, int _ch_num, double _p, int _max_ev_num);

    Queueing_system(const Th_values& _th_vs, int _max_ev_num);

    ~Queueing_system();

    void simulate(); // Метод моделирования работы СМО c заданными параметрами

    void simulate(double lambda,
                  double mu); // Метод моделирования работы СМО c изменённой интенсивностью

    void simulate(std::ostream& events_stream, std::ostream& requests_stream);
    // Метод моделирования работы СМО c заданными параметрами
    // и выводом данных в поток

    void simulate(double eps);
    // Метод моделирования работы СМО c заданной точностью

    //Константные методы доступа к полям класса
    Th_values getTheory() const;
    Exp_values getExperimental() const;
    double_pair getWait() const;
    double_pair getUtility() const;
    double_pair getZ() const;
    double_pair getLambda() const;
    double_pair getMu() const;
    double_pair getProp() const;
    double getThLambda() const;
    double getThMu() const;
    double getThProp() const;
    double_pair getThWait() const;
    double_pair getThUtility() const;
    int getChNum() const;
    double getLoad() const;
    int_pair s_status() const;
    int_pair q_status() const;
    int_pair getReqCount() const;
    int getReqNum() const;
    int getEvNum() const;
    int getMaxEvNum() const;

private:               // Поля класса
    Th_values th_vs;   // Сохраняем значения параметров системы
    Exp_values exp_vs; // Результаты симуляции работы
    std::unique_ptr<Req_flow> req_gen;
    std::unique_ptr<Req_queue> queue;
    std::vector<Request> channels;
    int_pair status; // Количество заявок каждого типа, находящихся на обслуживании
    int max_ev_num;  // Количество генерируемых событий

    auto free_min();
};
#endif // queueing_system_H
