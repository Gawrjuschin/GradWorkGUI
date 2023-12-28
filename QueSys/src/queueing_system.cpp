#include "queueing_system.h"

#include "requests_flow.h"
#include "requests_queue.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

Queueing_system::Queueing_system(double _lambda, double _mu, int _ch_num, double _p, int _max_ev_num)
    : th_vs(_lambda, _mu, _ch_num, _p)
    , exp_vs{}
    , req_gen(new RequestsFlow(_lambda, _mu, _p))
    , queue(new RequestsQueue)
    , channels(_ch_num)
    , system_status{0, 0}
    , max_ev_num{_max_ev_num}
{ //Проверку предоставляю вызывающему
    th_vs.theory();
}
Queueing_system::Queueing_system(const Th_values& _th_vs, int _max_ev_num)
    : th_vs(_th_vs)
    , exp_vs{}
    , req_gen(new RequestsFlow(_th_vs.L(), _th_vs.M(), _th_vs.P()))
    , queue(new RequestsQueue)
    , channels(_th_vs.N())
    , system_status{0, 0}
    , max_ev_num{_max_ev_num}
{ //Проверку предоставляю вызывающему
    th_vs.theory();
}

Queueing_system::~Queueing_system() = default;

Th_values Queueing_system::getTheory() const
{
    return th_vs;
};
Exp_values Queueing_system::getExperimental() const
{
    return exp_vs;
}
double_pair Queueing_system::getWait() const
{
    return exp_vs.W();
}
double_pair Queueing_system::getUtility() const
{
    return exp_vs.U();
}
double_pair Queueing_system::getZ() const
{
    return exp_vs.Z();
}
double_pair Queueing_system::getLambda() const
{
    return exp_vs.L();
}
double_pair Queueing_system::getMu() const
{
    return exp_vs.M();
}
double_pair Queueing_system::getProp() const
{
    return exp_vs.P();
}
double Queueing_system::getThLambda() const
{
    return th_vs.L();
}
double Queueing_system::getThMu() const
{
    return th_vs.M();
}
double Queueing_system::getThProp() const
{
    return th_vs.P();
}
double_pair Queueing_system::getThWait() const
{
    return th_vs.W();
}
double_pair Queueing_system::getThUtility() const
{
    return th_vs.U();
}
int Queueing_system::getChNum() const
{
    return th_vs.N();
}
double Queueing_system::getLoad() const
{
    return th_vs.Load();
}
int_pair Queueing_system::s_status() const
{
    return system_status;
}
int_pair Queueing_system::q_status() const
{
    return queue->status();
}
int_pair Queueing_system::getReqCount() const
{
    return exp_vs.Req_count();
}
// Заявок генерируется на 1 больше, а счётчик событий спешит на 1
int Queueing_system::getReqNum() const
{
    return exp_vs.Req_num() - 1;
}
int Queueing_system::getEvNum() const
{
    return exp_vs.Ev_num() - 1;
}
int Queueing_system::getMaxEvNum() const
{
    return max_ev_num;
}

auto Queueing_system::free_min()
{
    auto free = channels.end();
    auto min = channels.begin();
    for (auto iter = channels.begin(); iter != channels.end(); ++iter) {
        min = (iter->serve_end != -1)
                      && (iter->serve_end < min->serve_end || min->type == RequestType::kNone)
                  ? iter
                  : min;
        free = (free == channels.end() && iter->type == RequestType::kNone) ? iter : free;
    }
    return make_pair(free, min);
}

void Queueing_system::simulate(
    double lambda, double mu) //Метод моделирования работы СМО c изменённой интенсивностью
{ // Метод, запускающий симуляцию с другими параметрами интенсивности
    th_vs.rL() = lambda;
    th_vs.rM() = mu;
    th_vs.theory();
    queue->clear();
    std::unique_ptr<RequestsFlow> ptr(req_gen.release());
    req_gen.reset(new RequestsFlow(th_vs.L(), th_vs.M(), th_vs.P()));
    simulate();
}

void Queueing_system::simulate() //Нужно установить условие сходимости и добавить такую опцию в виджет
{                                //Симуляция для первых max_ev_num событий
    //Сброс системы (эксп. значения и каналы)
    Request free_channel{};
    for (auto& ch : channels) //Инициализируем каналы
    {
        ch = free_channel;
    }
    exp_vs = {}; //Сброс теоретических значений
    system_status = {0, 0};
    queue->clear(); //Сброс очереди
    //Ссылки как сокращения
    auto& evc = exp_vs.Ev_num();   //В систему поступила первая заявка
    auto& reqc = exp_vs.Req_num(); //Счётчик поступивших заявок
    auto& time = exp_vs.T();       //Замены (для краткости)
    auto& r = exp_vs.Req_count(); //Счётчик обработанных заявок по приоритетам
    auto& mu = exp_vs.Mu_sum();
    auto& w = exp_vs.W_sum();
    auto& wc = exp_vs.W_count();
    auto& z = exp_vs.Z_sum();
    RequestsFlow& req_generator = *req_gen;
    Request next_request = req_generator(reqc++); //Следующая заявка
    Event current_event; //Текущее событие. Постоянно меняется через метод set(...)
    double prev_time{next_request.arrive_time}; //Время предыдущего события
    int_pair prev_z{0, 0}; //Количество заявок до события для подсчёта среднего арифметического
    while (evc <= max_ev_num) {
        auto [free, min] = free_min();
        auto branch
            = (next_request.arrive_time < min->serve_end || min->type == RequestType::kNone)
                  //Попытка встать на обслуживание
                  ? ((free != channels.end()) ? ((!queue->empty()) ? 2 : 1) : 4)
                  // Есть свободный прибор "?" на обслуживание( очередь пуста "?" пришедшую "1" ":" первую в очереди "2")
                  // все заняты ":" в очередь "4"
                  : (((free != channels.end()) && !queue->empty()) ? 2 : 3);
        // Есть свободный прибор и заявка в очереди "?" на обслуживание первую в очереди "2" ":" освободить ближайшую заявку "3"
        switch (branch) {
        case 1: //Приборы простаивают, очередь пустая
        {       //Пришедшая заявка встаёт на обслуживание
            *free = next_request; //Первый свободный прибор принимает заявку
            next_request = req_generator(reqc++);
            time = free->arrive_time;
            free->queue_number = 0;
            free->ch_number = free - channels.begin() + 1; //Лучше distance
            free->start_time = free->arrive_time;
            free->serve_end = free->arrive_time + free->serve_time;
            free->wait_time = 0;
            (free->type == RequestType::kFirst)
                ? system_status.first++
                : system_status.second++; // Изменение количества заявок в СМО
            current_event = {.number = evc,
                             .time = time,
                             .type = (free->type == RequestType::kFirst) ? 1 : 2,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = free->number,
                             .time_next = next_request.arrive_time - free->arrive_time};
            break;
        }       //case 1
        case 2: //Есть свободный канал, очередь не пустая
        { //Первая в очереди заявка встаёт на обслуживание
            //Нужно проверить случай, когда Next_Request.second.time == min->serve_end
            (*free) = queue->front();
            (free->type == RequestType::kFirst)
                ? system_status.first++
                : system_status.second++; // Изменение количества заявок в СМО
            queue->pop();
            free->ch_number = std::distance(free, begin(channels)) + 1; //Лучше distance
            free->start_time = time;
            free->wait_time = time - free->arrive_time;
            free->serve_end = time + free->serve_time;
            current_event = {.number = evc,
                             .time = time,
                             .type = (free->type == RequestType::kFirst) ? 7 : 8,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = free->number,
                             .time_next = next_request.arrive_time
                                          - time}; //Запись данных в новое событие
            break;
        }       //case 2
        case 3: //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
        { //Освобождается прибор с минимальным оставшимся временем обслуживания
            time = min->serve_end; // Запоминается время выхода заявки. См. case 2:
            (min->type == RequestType::kFirst)
                ? system_status.first--
                : system_status.second--; // Изменение количества заявок в СМО
            current_event = {.number = evc,
                             .time = time,
                             .type = (min->type == RequestType::kFirst) ? 5 : 6,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = min->number,
                             .time_next = next_request.arrive_time - time};

            if (min->type == RequestType::kFirst) //(*min) - освободившаяся заявка
            { //Только здесь изменяется число заявок в системе
                mu.first += min->serve_time;
                r.first++;
                ((min->wait_time > 0) ? (w.first += min->wait_time, wc.first++) : 0);
            } else {
                mu.second += min->serve_time;
                r.second++;
                ((min->wait_time > 0) ? (w.second += min->wait_time, wc.second++) : 0);
            }
            (*min) = free_channel; // Освобождение прибора с минимальным оставшимся временем обслуживания
            break;
        }       //case 3
        case 4: //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
        { //Следующая заявка встаёт в очередь. Место в очереди вычисляется перед отправкой в неё
            double prev_arrive_time = next_request.arrive_time;
            next_request.queue_number = queue->status().first
                                        + (queue->status().second
                                           * (next_request.type == RequestType::kSecond))
                                        + 1;
            queue->push(next_request);
            time = next_request.arrive_time;
            next_request = req_generator(reqc++); //Вместо *min = Next_Req++;
            current_event = {.number = evc,
                             .time = time,
                             .type = (next_request.type == RequestType::kFirst) ? 3 : 4,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = (next_request.number - 1),
                             .time_next = next_request.arrive_time - prev_arrive_time};
            /*Номер пред.зааявки на 1 меньше*/
            break;
        } //case 4
        } //switch
        //Полностью заменяет проход по событиям из оригинальной работы
        z.first += (current_event.get_time() - prev_time)
                   * prev_z.first; //Теперь учитывается проведённое в состоянии время
        z.second += (current_event.get_time() - prev_time) * prev_z.second;
        prev_time = current_event.get_time(); //Запоминается время события
        prev_z = {current_event.get_q_status().first
                      + current_event.get_sys_status().first, //Запоминаем состояние
                  current_event.get_q_status().second + current_event.get_sys_status().second};
        evc++;
    } //while
    exp_vs.experimental();
    return;
}

void Queueing_system::simulate(std::ostream& events_stream, std::ostream& requests_stream)
{ //Симуляция для первых 1000 событий
    max_ev_num = 1000;

    events_stream << "Первые " << max_ev_num << " событий системы:\n";
    events_stream << "№\ttime\ttype\t(sys; que)\treq_num\ttime_next\n";
    requests_stream << "Обработанные заявки за первые " << max_ev_num << " событий\n";
    requests_stream << "№\ttype\tarrive\t\tqnum\tchnum\tstart\t\tserve\t\twait\t\tend\n";
    requests_stream << std::setiosflags(std::ios::fixed);
    requests_stream << std::setprecision(5);
    //Сброс системы (эксп. значения и каналы)
    for (auto& ch : channels) //Инициализируем каналы
    {
        ch = Request{};
    }
    exp_vs = {}; //Сброс теоретических значений
    system_status = {0, 0};
    queue->clear(); //Сброс очереди
    //Ссылки как сокращения
    auto& evc = exp_vs.Ev_num();   //В систему поступила первая заявка
    auto& reqc = exp_vs.Req_num(); //Счётчик поступивших заявок
    auto& time = exp_vs.T();       //Замены (для краткости)
    auto& r = exp_vs.Req_count(); //Счётчик обработанных заявок по приоритетам
    auto& mu = exp_vs.Mu_sum();
    auto& w = exp_vs.W_sum();
    auto& wc = exp_vs.W_count();
    auto& z = exp_vs.Z_sum();
    RequestsFlow& gen = *req_gen;
    Request next_request = gen(reqc++); //Следующая заявка
    Event current_event; //Текущее событие. Постоянно меняется через метод set(...)
    double prev_time{next_request.arrive_time}; //Время предыдущего события
    int_pair prev_z{0, 0}; //Количество заявок до события для подсчёта среднего арифметического
    while (evc <= max_ev_num) {
        auto [free, min] = free_min();
        auto branch
            = (next_request.arrive_time < min->serve_end || min->type == RequestType::kNone)
                  //Попытка встать на обслуживание
                  ? ((free != channels.end()) ? ((!queue->empty()) ? 2 : 1) : 4)
                  // Есть свободный прибор "?" на обслуживание( очередь пуста "?" пришедшую "1" ":" первую в очереди "2")
                  // все заняты ":" в очередь "4"
                  : (((free != channels.end()) && !queue->empty()) ? 2 : 3);
        // Есть свободный прибор и заявка в очереди "?" на обслуживание первую в очереди "2" ":" освободить ближайшую заявку "3"
        switch (branch) {
        case 1: //Приборы простаивают, очередь пустая
        {       //Пришедшая заявка встаёт на обслуживание
            *free = next_request; //Первый свободный прибор принимает заявку
            next_request = gen(reqc++);
            time = free->arrive_time;
            free->queue_number = 0;
            free->ch_number = free - channels.begin() + 1; //Лучше distance
            free->start_time = free->arrive_time;
            free->serve_end = free->arrive_time + free->serve_time;
            free->wait_time = 0;
            (free->type == RequestType::kFirst)
                ? system_status.first++
                : system_status.second++; // Изменение количества заявок в СМО

            current_event = {.number = evc,
                             .time = time,
                             .type = (free->type == RequestType::kFirst) ? 1 : 2,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = free->number,
                             .time_next = next_request.arrive_time - time};
            break;
        }       //case 1
        case 2: //Есть свободный канал, очередь не пустая
        { //Первая в очереди заявка встаёт на обслуживание
            //Нужно проверить случай, когда Next_Request.second.time == min->serve_end
            (*free) = queue->front();
            (free->type == RequestType::kFirst)
                ? system_status.first++
                : system_status.second++; // Изменение количества заявок в СМО
            queue->pop();
            free->ch_number = free - channels.begin() + 1; //Лучше distance
            free->start_time = time;
            free->wait_time = time - free->arrive_time;
            free->serve_end = time + free->serve_time;

            current_event = {.number = evc,
                             .time = time,
                             .type = (free->type == RequestType::kFirst) ? 7 : 8,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = free->number,
                             .time_next = next_request.arrive_time - time};
            break;
        }       //case 2
        case 3: //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
        { //Освобождается прибор с минимальным оставшимся временем обслуживания
            time = min->serve_end; // Запоминается время выхода заявки. См. case 2:
            (min->type == RequestType::kFirst)
                ? system_status.first--
                : system_status.second--; // Изменение количества заявок в СМО

            current_event = {.number = evc,
                             .time = time,
                             .type = (min->type == RequestType::kFirst) ? 5 : 6,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = min->number,
                             .time_next = next_request.arrive_time - time};

            if (min->type == RequestType::kSecond) //(*min) - освободившаяся заявка
            { //Только здесь изменяется число заявок в системе
                mu.second += min->serve_time;
                r.second++;
                ((min->wait_time > 0) ? (w.second += min->wait_time, wc.second++) : 0);
            } else {
                mu.first += min->serve_time;
                r.first++;
                ((min->wait_time > 0) ? (w.first += min->wait_time, wc.first++) : 0);
            }
            requests_stream << *min;
            (*min) = Request{}; // Освобождение прибора с минимальным оставшимся временем обслуживания
            break;
        }       //case 3
        case 4: //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
        { //Следующая заявка встаёт в очередь. Место в очереди вычисляется перед отправкой в неё
            double prev_arrive_time = next_request.arrive_time;
            next_request.queue_number = queue->status().first
                                        + (queue->status().second
                                           * (next_request.type == RequestType::kSecond))
                                        + 1;
            queue->push(next_request);
            time = next_request.arrive_time;
            next_request = gen(reqc++);

            current_event = {.number = evc,
                             .time = time,
                             .type = (next_request.type == RequestType::kFirst) ? 3 : 4,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = (next_request.number - 1),
                             .time_next = next_request.arrive_time - prev_arrive_time};
            break;
        } //case 4
        } //switch
        z.first += (current_event.get_time() - prev_time)
                   * prev_z.first; //Теперь учитывается проведённое в состоянии время
        z.second += (current_event.get_time() - prev_time) * prev_z.second;
        prev_time = current_event.get_time(); //Запоминается время события
        prev_z = {current_event.get_q_status().first
                      + current_event.get_sys_status().first, //Запоминаем состояние
                  current_event.get_q_status().second + current_event.get_sys_status().second};
        events_stream << current_event;
        ++evc;
    } //while
    for (auto& req : channels) {
        if (req.type != RequestType::kNone) {
            requests_stream << req;
        }
    }
    if (queue->empty()) {
        requests_stream << "\nОчередь пуста!\n";
    } else { // Записываем заявки в файл, поочерёдно вытаскивая из очереди, а потом возвращаем их обратно
        std::vector<Request> vec;
        vec.reserve(queue->status().first + queue->status().second);
        requests_stream << "\nЗаявки, оставшиеся в очереди:\n";
        while (!queue->empty()) {
            requests_stream << queue->front();
            vec.push_back(queue->front());
            queue->pop();
        }
        for (auto& x : vec) {
            queue->push(x);
        }
    } //Очередь опустошается!
    exp_vs.experimental();
}

void Queueing_system::simulate(
    double eps) //Нужно установить условие сходимости и добавить такую опцию в виджет
{               //Симуляция до сходимости по вероятности
    //Сброс системы (эксп. значения и каналы)
    Request free_channel{};
    for (auto& ch : channels) //Инициализируем каналы
    {
        ch = Request{};
    }
    exp_vs = {}; //Сброс теоретических значений
    system_status = {0, 0};
    queue->clear();          //Сброс очереди
    double_pair delta{1, 2}; //Дельта для подсчёта с заданной точностью
    //Ссылки как сокращения
    auto& evc = exp_vs.Ev_num();   //В систему поступила первая заявка
    auto& reqc = exp_vs.Req_num(); //Счётчик поступивших заявок
    auto& time = exp_vs.T();       //Замены (для краткости)
    auto& r = exp_vs.Req_count(); //Счётчик обработанных заявок по приоритетам
    auto& mu = exp_vs.Mu_sum();
    auto& w = exp_vs.W_sum();
    auto& wc = exp_vs.W_count();
    auto& z = exp_vs.Z_sum();
    RequestsFlow& gen = *req_gen;
    Request next_request = gen(reqc++); //Следующая заявка
    Event current_event; //Текущее событие. Постоянно меняется через метод set(...)
    double prev_time{next_request.arrive_time}; //Время предыдущего события
    int_pair prev_z{0, 0}; //Количество заявок до события для подсчёта среднего арифметического
    while (delta.second > eps) {
        auto [free, min] = free_min();
        auto branch
            = (next_request.arrive_time < min->serve_end || min->type == RequestType::kNone)
                  //Попытка встать на обслуживание
                  ? ((free != channels.end()) ? ((!queue->empty()) ? 2 : 1) : 4)
                  // Есть свободный прибор "?" на обслуживание( очередь пуста "?" пришедшую "1" ":" первую в очереди "2")
                  // все заняты ":" в очередь "4"
                  : (((free != channels.end()) && !queue->empty()) ? 2 : 3);
        // Есть свободный прибор и заявка в очереди "?" на обслуживание первую в очереди "2" ":" освободить ближайшую заявку "3"
        switch (branch) {
        case 1: //Приборы простаивают, очередь пустая
        {       //Пришедшая заявка встаёт на обслуживание
            *free = next_request; //Первый свободный прибор принимает заявку
            next_request = gen(reqc++);
            time = free->arrive_time;
            free->queue_number = 0;
            free->ch_number = free - channels.begin() + 1; //Лучше distance
            free->start_time = free->arrive_time;
            free->serve_end = free->arrive_time + free->serve_time;
            free->wait_time = 0;
            (free->type == RequestType::kFirst)
                ? system_status.first++
                : system_status.second++; // Изменение количества заявок в СМО
            current_event = {.number = evc,
                             .time = time,
                             .type = (free->type == RequestType::kFirst) ? 1 : 2,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = free->number,
                             .time_next = next_request.arrive_time - free->arrive_time};
            break;
        }       //case 1
        case 2: //Есть свободный канал, очередь не пустая
        { //Первая в очереди заявка встаёт на обслуживание
            //Нужно проверить случай, когда Next_Request.second.time == min->serve_end
            (*free) = queue->front();
            (free->type == RequestType::kFirst)
                ? system_status.first++
                : system_status.second++; // Изменение количества заявок в СМО
            queue->pop();
            free->ch_number = free - channels.begin() + 1; //Лучше distance
            free->start_time = time;
            free->wait_time = time - free->arrive_time;
            free->serve_end = time + free->serve_time;
            current_event = {.number = evc,
                             .time = time,
                             .type = (free->type == RequestType::kFirst) ? 7 : 8,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = free->number,
                             .time_next = next_request.arrive_time - time};
            break;
        }       //case 2
        case 3: //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
        { //Освобождается прибор с минимальным оставшимся временем обслуживания
            time = min->serve_end; // Запоминается время выхода заявки. См. case 2:
            (min->type == RequestType::kFirst)
                ? system_status.first--
                : system_status.second--; // Изменение количества заявок в СМО
            current_event = {.number = evc,
                             .time = time,
                             .type = (min->type == RequestType::kFirst) ? 5 : 6,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = min->number,
                             .time_next = next_request.arrive_time - time};
            if (min->type == RequestType::kSecond) //(*min) - освободившаяся заявка
            { //Только здесь изменяется число заявок в системе
                mu.second += min->serve_time;
                r.second++;
                ((min->wait_time > 0) ? (w.second += min->wait_time, wc.second++) : 0);
            } else {
                mu.first += min->serve_time;
                r.first++;
                ((min->wait_time > 0) ? (w.first += min->wait_time, wc.first++) : 0);
            }
            auto next_delta = std::abs(delta.first - (double(r.first) / (r.first + r.second)));
            if (next_delta < delta.second && next_delta != 0) { //Это минимальная дельта
                delta.second = next_delta;
            }
            delta.first = double(r.first) / //Вычисляем текущее значение приближения С.В.
                          (r.first + r.second);
            (*min) = free_channel; // Освобождение прибора с минимальным оставшимся временем обслуживания
            break;
        }       //case 3
        case 4: //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
        { //Следующая заявка встаёт в очередь. Место в очереди вычисляется перед отправкой в неё
            double prev_arrive_time = next_request.arrive_time;
            next_request.queue_number = queue->status().first
                                        + (queue->status().second
                                           * (next_request.type == RequestType::kSecond))
                                        + 1;
            queue->push(next_request);
            time = next_request.arrive_time;
            next_request = gen(reqc++); //Вместо *min = Next_Req++;
            current_event = {.number = evc,
                             .time = time,
                             .type = (next_request.type == RequestType::kFirst) ? 3 : 4,
                             .system_status = system_status,
                             .queue_status = queue->status(),
                             .request = (next_request.number - 1),
                             .time_next = next_request.arrive_time - prev_arrive_time};
            /*Номер пред.заявки на 1 меньше*/
            break;
        } //case 4
        } //switch
        z.first += (current_event.get_time() - prev_time)
                   * prev_z.first; //Теперь учитывается проведённое в состоянии время
        z.second += (current_event.get_time() - prev_time) * prev_z.second;
        prev_time = current_event.get_time(); //Запоминается время события
        prev_z = {current_event.get_q_status().first
                      + current_event.get_sys_status().first, //Запоминаем состояние
                  current_event.get_q_status().second + current_event.get_sys_status().second};
        evc++;
    } //while
    exp_vs.experimental();
}

namespace queueing_system {

auto FreeMin(std::vector<Request>& channels)
{
    auto free = end(channels);
    auto min = begin(channels);
    for (auto it = begin(channels); it != end(channels); ++it) {
        if (!IsEmpty(*it)) {
            if (IsEmpty(*min) || it->serve_end < min->serve_end) {
                min = it;
            }
        } else {
            if (free == end(channels)) {
                free = it;
            }
        }
    }
    return make_pair(free, min);
}

SimulationResult CalcResult(const int request_number,
                            const int event_number,
                            const double time_total,
                            const std::pair<int, int> requests_total,
                            const std::pair<double, double> serve_total,
                            const std::pair<int, int> requests_served,
                            const std::pair<double, double> wait_total,
                            const std::pair<int, int> requests_waited)
{
    SimulationResult result{};

    result.requests = request_number, result.events = event_number,
    result.avg_period = {time_total / requests_total.first, time_total / requests_total.second},
    result.avg_serve = {serve_total.first / requests_served.first,
                        serve_total.second / requests_served.second},
    result.propability = {requests_total.first / (requests_total.first + requests_total.second),
                          requests_total.second / (requests_total.first + requests_total.second)},
    result.avg_wait = {wait_total.first / requests_waited.first,
                       wait_total.second / requests_waited.second},
    result.avg_requests = {requests_total.first / time_total, requests_total.second / time_total};

    result.avg_utility.first = result.avg_wait.first + result.avg_serve.first;
    result.avg_utility.second = result.avg_wait.second + result.avg_serve.second;

    return result;
}

SimulationResult Simulate(double lambda_th,
                          double mu_th,
                          int channels_number,
                          double prop,
                          int max_events,
                          std::function<void(const Event&)> write_event = nullptr,
                          std::function<void(const Request&)> write_request = nullptr)
{
    RequestsFlow req_generator(lambda_th, mu_th, prop);

    std::vector<Request> channels(channels_number);
    RequestsQueue queue{};

    double time_total{};
    std::pair<double, double> serve_total{};
    std::pair<double, double> wait_total{};
    std::pair<int, int> requests_total{};
    std::pair<int, int> requests_served{};
    std::pair<int, int> requests_waited{};
    std::pair<int, int> system_status{};

    // Фактически количество сгенерированных заявок
    int request_number{};
    // Очередная заявка
    Request next_request = req_generator(++request_number);

    // В систему поступила первая заявка
    // Номер последнего события
    int event_number{1};
    for (; event_number <= max_events; ++event_number) {
        Event current_event{};
        auto [free_it, min_it] = FreeMin(channels);

        // TODO: разбить на отдельные функции
        if (free_it != end(channels)) {
            // Есть свободный канал
            if (queue.empty()) {
                // Очередная заявка встаёт на обслуживание
                // Первый свободный прибор принимает заявку
                time_total = next_request.arrive_time;

                *free_it = next_request;
                (next_request.type == RequestType::kFirst) ? ++requests_total.first
                                                           : ++requests_total.second;

                next_request = req_generator(++request_number);
                free_it->queue_number = 0;
                free_it->ch_number = std::distance(free_it, begin(channels)) + 1;
                free_it->start_time = free_it->arrive_time;
                free_it->serve_end = free_it->arrive_time + free_it->serve_time;
                free_it->wait_time = 0;

                // Изменение количества заявок в СМО
                // Заменить тип на енам
                (free_it->type == RequestType::kFirst) ? ++system_status.first
                                                       : ++system_status.second;

                current_event = {.number = event_number,
                                 .time = time_total,
                                 .type = (free_it->type == RequestType::kFirst) ? 1 : 2,
                                 .system_status = system_status,
                                 .queue_status = queue.status(),
                                 .request = free_it->number,
                                 .time_next = next_request.arrive_time - free_it->arrive_time};
            } else {
                //Первая в очереди заявка встаёт на обслуживание
                //Первый свободный прибор принимает заявку
                (*free_it) = queue.front();

                // Изменение количества заявок в СМО
                (free_it->type == RequestType::kFirst) ? ++system_status.first
                                                       : ++system_status.second;
                queue.pop();
                free_it->ch_number = std::distance(free_it, begin(channels)) + 1;
                free_it->start_time = time_total;
                free_it->wait_time = time_total - free_it->arrive_time;
                free_it->serve_end = time_total + free_it->serve_time;

                //Запись данных в новое событие
                current_event = {.number = event_number,
                                 .time = time_total,
                                 .type = (free_it->type == RequestType::kFirst) ? 7 : 8,
                                 .system_status = system_status,
                                 .queue_status = queue.status(),
                                 .request = free_it->number,
                                 .time_next = next_request.arrive_time - time_total};
            }
        } else {
            // Все приборы заняты
            assert(!IsEmpty(*min_it));
            if (min_it->serve_end <= next_request.arrive_time) {
                //  Канал с минимальным оставшимся временем обслуживания освободится раньше прихода очередной заявки
                //  Освобождается канал с минимальным оставшимся временем обслуживания.
                time_total = min_it->serve_end;

                // Изменение количества заявок в СМО
                (min_it->type == RequestType::kFirst) ? --system_status.first
                                                      : --system_status.second;
                //Запись данных в новое событие
                current_event = {.number = event_number,
                                 .time = time_total,
                                 .type = (min_it->type == RequestType::kFirst) ? 5 : 6,
                                 .system_status = system_status,
                                 .queue_status = queue.status(),
                                 .request = min_it->number,
                                 .time_next = next_request.arrive_time - time_total};

                //(*min_it) - освободившаяся заявка
                //Только здесь изменяется число обработанных заявок
                if (min_it->type == RequestType::kFirst) {
                    serve_total.first += min_it->serve_time;
                    requests_served.first++;
                    if (min_it->wait_time > 0) {
                        wait_total.first += min_it->wait_time;
                        requests_waited.first++;
                    }
                } else {
                    serve_total.second += min_it->serve_time;
                    requests_served.second++;
                    if (min_it->wait_time > 0) {
                        wait_total.second += min_it->wait_time;
                        requests_waited.second++;
                    }
                }

                // Обработанная заявка отправляется в поток вывода
                if (write_request) {
                    write_request(*min_it);
                }

                // Освобождение прибора с минимальным оставшимся временем обслуживания
                (*min_it) = Request{};
            } else {
                //  Очередная заявка прийдёт раньше, чем освободится канал с минимальным оставшимся временем обслуживания
                //  Очередная заявка встаёт в очередь.
                time_total = next_request.arrive_time;

                double prev_arrive_time = next_request.arrive_time;
                next_request.queue_number = queue.status().first
                                            + (queue.status().second
                                               * (next_request.type == RequestType::kSecond))
                                            + 1;
                queue.push(next_request);
                (next_request.type == RequestType::kFirst) ? ++requests_total.first
                                                           : ++requests_total.second;

                next_request = req_generator(++request_number);
                //Запись данных в новое событие
                current_event = {.number = event_number,
                                 .time = time_total,
                                 .type = (next_request.type == RequestType::kFirst) ? 3 : 4,
                                 .system_status = system_status,
                                 .queue_status = queue.status(),
                                 .request = (next_request.number - 1),
                                 .time_next = next_request.arrive_time - prev_arrive_time};
            }
        }

        // Каждое ветвление порождает событие
        if (write_event) {
            write_event(current_event);
        }
    } //for

    if (write_request) {
        for (auto& req : channels) {
            if (!IsEmpty(req)) {
                write_request(req);
            }
        }
        // Записываем оставшиеся в очереди заявки. Очередь опустошается!
        while (!queue.empty()) {
            write_request(queue.front());
            queue.pop();
        }
    }

    return CalcResult(request_number,
                      event_number,
                      time_total,
                      requests_total,
                      serve_total,
                      requests_served,
                      wait_total,
                      requests_waited);
}

} // namespace queueing_system
