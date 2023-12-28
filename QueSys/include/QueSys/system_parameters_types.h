#ifndef SYSTEM_PARAMETERS_TYPES_H
#define SYSTEM_PARAMETERS_TYPES_H

#include <sstream>
#include <string>
#include <utility>

using int_pair = std::pair<int, int>;
using double_pair = std::pair<double, double>;

class Th_values
{
public:
    Th_values() = default;

    Th_values(double _lambda, double _mu, int _ch_num, double _p)
        : lambda{_lambda}
        , mu{_mu}
        , ch_num{_ch_num}
        , prop{_p}
        , w({0, 0})
        , u({0, 0})
    {
        theory();
    }

    Th_values(const Th_values& _th_vals) = default;
    Th_values& operator=(const Th_values&) = default;
    Th_values(Th_values&& _th_vals) = default;
    Th_values& operator=(Th_values&& _th_vals) = default;

    ~Th_values() = default;

    double& rL() { return lambda; }
    double& rM() { return mu; }
    double L() const { return lambda; }
    double M() const { return mu; }
    int N() const { return ch_num; }
    double P() const { return prop; }
    double Load() const { return lambda / mu / ch_num; }
    double_pair W() const { return w; }
    double_pair U() const { return u; }

    std::string file_prefix()
    { //Префикс с параметрами для генерируемого файла формата l_m_n_p_
        std::stringstream temp;
        temp << lambda << '_' << mu << '_' << ch_num << '_' << prop << '_';
        return temp.str();
    }

    void theory()
    {
        w.first = (lambda) / (mu * ((ch_num * mu) - (prop * lambda)));
        w.second = (w.first * mu * ch_num) / ((ch_num * mu) - lambda);
        u.first = w.first + 1 / mu;
        u.second = w.second + 1 / mu;
    }

private:
    //Теоретические значения:
    double lambda{}; //Интенсивность входного потока
    double mu{};     //Интенсивность потока обслуживания
    int ch_num{};    //Число приборов
    double prop{}; //Вероятность того, что заявка окажется первого типа
    double_pair w{}; //Оценка среднего времени ожидания в очереди заявок 2-х приоритетов
    double_pair u{}; //Оценка среднего времени пребывания в СМО заявок 2-х приоритетов
};

class Exp_values
{
public:
    Exp_values() = default;
    Exp_values(const Exp_values& rhval) = default;
    Exp_values& operator=(const Exp_values& rhval) = default;
    Exp_values(Exp_values&& rhval) = default;
    Exp_values& operator=(Exp_values&& rhval) = default;
    ~Exp_values() = default;

    void experimental()
    {
        lambda_pair.first = r_count.first / time;
        lambda_pair.second = r_count.second / time;
        prop_pair.first = r_count.first / (r_count.first + r_count.second);
        prop_pair.second = r_count.second / (r_count.first + r_count.second);
        mu_pair.first = r_count.first / temp_mu.first;
        mu_pair.second = r_count.second / temp_mu.second;
        w.first = //Использую temp_w чтобы можно было считать
            temp_w.first / w_count.first; //повторно через N итераций
        w.second = temp_w.second / w_count.second;
        u.first = w.first + 1 / mu_pair.first;
        u.second = w.second + 1 / mu_pair.second;
        z.first = temp_z.first / time;
        z.second = temp_z.second / time;
    }

    double_pair W() const { return w; }
    double_pair U() const { return u; }
    double_pair Z() const { return z; }
    double_pair L() const { return lambda_pair; }
    double_pair M() const { return mu_pair; }
    double_pair P() const { return prop_pair; }
    double_pair& Mu_sum() { return temp_mu; }
    double_pair& W_sum() { return temp_w; }
    int_pair& W_count() { return w_count; }
    double_pair& Z_sum() { return temp_z; }
    //~~~~~~~~~~~~~~~~~~~~~~~~
    double& T() { return time; }
    int& Ev_num() { return evnum; }
    int Ev_num() const { return evnum; }
    int& Req_num() { return reqnum; }
    int Req_num() const { return reqnum; }
    int_pair& Req_count() { return r_count; }
    int_pair Req_count() const { return r_count; }

private:
    //Экспериментальные зачения:
    int_pair r_count{0, 0}; //Количество заявок обоих типов, обработанных в СМО
    double_pair
        lambda_pair{0, 0}; //Экспериментальная интенсивность входного потока заявок 2-х приоритетов
    double_pair
        mu_pair{0, 0}; //Экспериментальная интенсивность потока обслуживания заявок 2-х приоритетов
    double_pair temp_mu{0, 0};   //mu для вычисления "на лету"
    double_pair prop_pair{0, 0}; //Доля заявок первого типа
    double_pair w{0, 0}; //Экспериментальное среднее время ожидания в очереди заявок 2-х приоритетов
    double_pair temp_w{0, 0}; //W для вычисления "на лету"
    int_pair w_count{0, 0}; //Количество заявок каждого приоритета, простоявших в очереди
    double_pair u{0, 0}; //Экспериментальное среднее время пребывания в СМО заявок 2-х приоритетов
    double_pair z{0, 0}; //Экспериментальное значение среднего числа заявок в СМО.
    double_pair temp_z{0, 0}; //Z для вычисления "на лету".
    double time{0};           //Время события
    int evnum{1};             //Нормер события
    int reqnum{1};            //Номер сгенерированной заявки
};

#endif // SYSTEM_PARAMETERS_TYPES_H
