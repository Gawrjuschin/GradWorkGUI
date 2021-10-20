#ifndef SYSTEM_PARAMETERS_TYPES_H
#define SYSTEM_PARAMETERS_TYPES_H

#include <utility>
#include <string>
#include <sstream>

using   ipair = std::pair<int,int>;
using   dpair = std::pair<double,double>;

class Th_values
{

public:
  Th_values() = default;

  Th_values(double _lambda, double _mu, int _ch_num, double _p)
    : lambda{_lambda}, mu{_mu}, ch_num{_ch_num}
    , prop{_p}, w({0,0}),u({0,0})
  {
    theory();
  }

  Th_values(const Th_values& _th_vals) = default;
  Th_values& operator =(const Th_values&) = default;
  Th_values(Th_values&& _th_vals) = default;
  Th_values& operator =(Th_values&& _th_vals) = default;

  double& rL() {return lambda;}
  double& rM() {return mu;}
  double L() const {return lambda;}
  double M() const {return mu;}
  int    N() const {return ch_num;}
  double P() const {return prop;}
  double Load() const {return lambda / mu / ch_num;}
  dpair  W() const {return w;}
  dpair  U() const {return u;}

  std::string file_prefix()
  {//Префикс с параметрами для генерируемого файла формата l_m_n_p_
    std::stringstream temp;
    temp << lambda << '_' << mu << '_' << ch_num << '_' << prop << '_';
    return temp.str();
  }

  void theory()
  {
    w.first   = (lambda) / (mu * ((ch_num * mu) - (prop * lambda)));
    w.second  = (w.first * mu * ch_num) / ((ch_num * mu) - lambda);
    u.first   = w.first + 1 / mu;
    u.second  = w.second + 1 / mu;
  }

private:
  //Теоретические значения:
    double lambda;  //Интенсивность входного потока
    double mu;      //Интенсивность потока обслуживания
    int    ch_num;  //Число приборов
    double prop;    //Вероятность того, что заявка окажется первого типа
    dpair  w;       //Оценка среднего времени ожидания в очереди заявок 2-х приоритетов
    dpair  u;       //Оценка среднего времени пребывания в СМО заявок 2-х приоритетов
};

class Exp_values
{

public:
  Exp_values() = default;
  Exp_values(const Exp_values& rhval) = default;
  Exp_values& operator =(const Exp_values& rhval) = default;
  Exp_values(Exp_values&& rhval) = default;
  Exp_values& operator =(Exp_values&& rhval) = default;
  ~Exp_values() = default;


  void experimental()
  {
    lambda_pair.first =
        r_count.first / time;
    lambda_pair.second =
        r_count.second / time;
    prop_pair.first =
        r_count.first /
        (r_count.first + r_count.second);
    prop_pair.second =
        r_count.second /
        (r_count.first + r_count.second);
    mu_pair.first =
        r_count.first / temp_mu.first;
    mu_pair.second =
        r_count.second / temp_mu.second;
    w.first = //Использую temp_w чтобы можно было считать
        temp_w.first / w_count.first; //повторно через N итераций
    w.second =
        temp_w.second / w_count.second;
    u.first =
        w.first + 1 / mu_pair.first;
    u.second =
        w.second + 1 / mu_pair.second;
    z.first = temp_z.first / time;
    z.second = temp_z.second / time;
  }

  dpair   W() const {return w;}
  dpair   U() const {return u;}
  dpair   Z() const {return z;}
  dpair   L() const {return lambda_pair;}
  dpair   M() const {return mu_pair;}
  dpair   P() const {return prop_pair;}
  dpair&  Mu_sum()  {return temp_mu;}
  dpair&  W_sum()   {return temp_w;}
  ipair&  W_count() {return w_count;}
  dpair&  Z_sum()   {return temp_z;}
  //~~~~~~~~~~~~~~~~~~~~~~~~
  double& T() {return time;}
  int&    Ev_num() {return evnum;}
  int     Ev_num() const {return evnum;}
  int&    Req_num() {return reqnum;}
  int     Req_num() const {return reqnum;}
  ipair&  Req_count() {return r_count;}
  ipair   Req_count() const {return r_count;}


private:
  //Экспериментальные зачения:
    ipair   r_count{0,0};//Количество заявок обоих типов, обработанных в СМО
    dpair   lambda_pair{0,0};//Экспериментальная интенсивность входного потока заявок 2-х приоритетов
    dpair   mu_pair{0,0};//Экспериментальная интенсивность потока обслуживания заявок 2-х приоритетов
    dpair   temp_mu{0,0};//mu для вычисления "на лету"
    dpair   prop_pair{0,0};//Доля заявок первого типа
    dpair   w{0,0};//Экспериментальное среднее время ожидания в очереди заявок 2-х приоритетов
    dpair   temp_w{0,0};//W для вычисления "на лету"
    ipair   w_count{0,0};//Количество заявок каждого приоритета, простоявших в очереди
    dpair   u{0,0};//Экспериментальное среднее время пребывания в СМО заявок 2-х приоритетов
    dpair   z{0,0}; //Экспериментальное значение среднего числа заявок в СМО.
    dpair   temp_z{0,0}; //Z для вычисления "на лету".
    double  time{0};//Время события
    int     evnum{1};//Нормер события
    int     reqnum{1};//Номер сгенерированной заявки
};

#endif // SYSTEM_PARAMETERS_TYPES_H
