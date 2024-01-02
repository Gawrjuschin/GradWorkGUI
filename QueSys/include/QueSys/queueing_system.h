#ifndef queueing_system_H
#define queueing_system_H

#include "event.h"
#include "request.h"
#include "system_parameters_types.h"
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using int_pair = std::pair<int, int>;
using double_pair = std::pair<double, double>;

class RequestsFlow;
class RequestsQueue;

class Queueing_system
{
public:
    Queueing_system() = delete;

    Queueing_system(double _lambda, double _mu, int _ch_num, double _p, int _max_ev_num);

    Queueing_system(const Th_values& _th_vs, int _max_ev_num);

    Queueing_system(const Queueing_system&) = delete;
    Queueing_system& operator=(const Queueing_system&) = delete;

    Queueing_system(Queueing_system&&) = delete;
    Queueing_system& operator=(Queueing_system&&) = delete;

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
    std::unique_ptr<RequestsFlow> req_gen;
    std::unique_ptr<RequestsQueue> queue;
    std::vector<Request> channels;
    int_pair system_status; // Количество заявок каждого типа, находящихся на обслуживании
    int max_ev_num;  // Количество генерируемых событий

    auto free_min();
};

namespace queueing_system {

struct SimulationResult
{
    double time_passed{};
    int events{};
    int requests{};
    std::pair<double, double> avg_period;
    std::pair<double, double> avg_serve;
    std::pair<double, double> propability;
    std::pair<double, double> avg_wait;
    std::pair<double, double> avg_utility;
    std::pair<double, double> avg_requests;
};

SimulationResult Simulate(double lambda_th,
                          double mu_th,
                          int channels_number,
                          double prop,
                          int max_events,
                          std::function<void(const Event&)> write_event = nullptr,
                          std::function<void(const Request&)> write_request = nullptr);
// TODO: реализовать перегрузку с ограничением на eps

} // namespace queueing_system

#endif // queueing_system_H
