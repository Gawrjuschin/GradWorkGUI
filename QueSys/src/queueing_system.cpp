#include "queueing_system.h"

#include "requests_flow.h"
#include "requests_queue.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include <cassert>
#include <iostream>

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
        case 1: {
            //Приборы простаивают, очередь пустая
            //Пришедшая заявка встаёт на обслуживание
            //Первый свободный прибор принимает заявку
            *free = next_request;
            next_request = req_generator(reqc++);
            time = free->arrive_time;
            free->queue_number = 0;
            free->ch_number = std::distance(begin(channels), free) + 1;
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
        } //case 1
        case 2: {
            //Есть свободный канал, очередь не пустая
            //Первая в очереди заявка встаёт на обслуживание
            //Нужно проверить случай, когда Next_Request.second.time == min->serve_end
            (*free) = queue->front();
            (free->type == RequestType::kFirst)
                ? system_status.first++
                : system_status.second++; // Изменение количества заявок в СМО
            queue->pop();
            free->ch_number = std::distance(begin(channels), free) + 1;
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
        } //case 2
        case 3: {
            //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            //Освобождается прибор с минимальным оставшимся временем обслуживания
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
        } //case 3
        case 4: {
            //Все приборы заняты, минимальное время окончания обслуживания меньше время прихода следующей заявки
            //Следующая заявка встаёт в очередь. Место в очереди вычисляется перед отправкой в неё
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
            free->ch_number = std::distance(begin(channels), free) + 1;
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
            free->ch_number = std::distance(begin(channels), free) + 1;
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
            free->ch_number = std::distance(begin(channels), free) + 1;
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
            free->ch_number = std::distance(begin(channels), free) + 1;
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

static inline auto FreeMin(std::vector<Request>& channels) noexcept
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

struct SimulationStatus
{
    // Номер очередной заявки. Фактически количество сгенерированных заявок
    int request_number{0};
    // Номер последнего события. Фактически количество сгенерированных событий
    int event_number{0};

    double time_passed{};
    std::pair<double, double> serve_total{};
    std::pair<double, double> wait_total{};
    std::pair<int, int> requests_total{};
    std::pair<int, int> requests_served{};
    std::pair<int, int> requests_waited{};
    // Среднее число заяволк в системе вычисляется на ходу
    std::pair<double, double> requests_weighted_summ{};
};

static inline auto CalcResult(const SimulationStatus& simulation_status) noexcept
{
    SimulationResult result{};

    result.time_passed = simulation_status.time_passed;
    result.requests = simulation_status.request_number;
    result.events = simulation_status.event_number;
    result.avg_period = {simulation_status.time_passed / simulation_status.requests_total.first,
                         simulation_status.time_passed / simulation_status.requests_total.second};
    result.avg_serve = {simulation_status.serve_total.first
                            / simulation_status.requests_served.first,
                        simulation_status.serve_total.second
                            / simulation_status.requests_served.second};
    result.propability = {simulation_status.requests_total.first
                              / (simulation_status.requests_total.first
                                 + simulation_status.requests_total.second),
                          simulation_status.requests_total.second
                              / (simulation_status.requests_total.first
                                 + simulation_status.requests_total.second)};
    result.avg_wait = {simulation_status.wait_total.first / simulation_status.requests_waited.first,
                       simulation_status.wait_total.second
                           / simulation_status.requests_waited.second};
    result.avg_requests = {simulation_status.requests_weighted_summ.first
                               / simulation_status.time_passed,
                           simulation_status.requests_weighted_summ.second
                               / simulation_status.time_passed};

    result.avg_utility = {result.avg_wait.first + result.avg_serve.first,
                          result.avg_wait.second + result.avg_serve.second};

    return result;
}

// 1)
// Очередная заявка встаёт на обслуживание
// Первый свободный прибор принимает заявку
static inline auto StartNext(RequestsFlow& req_generator,
                             Request& next_request,
                             std::vector<Request>& channels,
                             RequestsQueue& queue,
                             std::pair<int, int>& system_status,
                             SimulationStatus& simulation_status,
                             std::vector<Request>::iterator free_it,
                             std::vector<Request>::iterator min_it) -> Event
{
    simulation_status.time_passed = next_request.arrive_time;

    *free_it = next_request;
    // Новая заявка в системе
    (next_request.type == RequestType::kFirst) ? ++simulation_status.requests_total.first
                                               : ++simulation_status.requests_total.second;

    next_request = req_generator(++simulation_status.request_number);
    free_it->queue_number = 0;
    free_it->ch_number = std::distance(begin(channels), free_it) + 1;
    free_it->start_time = free_it->arrive_time;
    free_it->serve_end = free_it->arrive_time + free_it->serve_time;
    free_it->wait_time = 0;

    // Изменение количества заявок в СМО
    // Заменить тип на енам
    (free_it->type == RequestType::kFirst) ? ++system_status.first : ++system_status.second;

    return {.number = simulation_status.event_number,
            .time = simulation_status.time_passed,
            .type = (free_it->type == RequestType::kFirst) ? 1 : 2,
            .system_status = system_status,
            .queue_status = queue.status(),
            .request = free_it->number,
            .time_next = next_request.arrive_time - free_it->arrive_time};
};

// 2)
//Первая в очереди заявка встаёт на обслуживание
//Первый свободный прибор принимает заявку
static inline auto StartQueued(RequestsFlow& req_generator,
                               Request& next_request,
                               std::vector<Request>& channels,
                               RequestsQueue& queue,
                               std::pair<int, int>& system_status,
                               SimulationStatus& simulation_status,
                               std::vector<Request>::iterator free_it,
                               std::vector<Request>::iterator min_it) -> Event
{
    (*free_it) = queue.front();

    // Изменение количества заявок в СМО
    (free_it->type == RequestType::kFirst) ? ++system_status.first : ++system_status.second;
    queue.pop();
    free_it->ch_number = std::distance(begin(channels), free_it) + 1;
    free_it->start_time = simulation_status.time_passed;
    free_it->wait_time = simulation_status.time_passed - free_it->arrive_time;
    free_it->serve_end = simulation_status.time_passed + free_it->serve_time;

    //Запись данных в новое событие
    return {.number = simulation_status.event_number,
            .time = simulation_status.time_passed,
            .type = (free_it->type == RequestType::kFirst) ? 7 : 8,
            .system_status = system_status,
            .queue_status = queue.status(),
            .request = free_it->number,
            .time_next = next_request.arrive_time - simulation_status.time_passed};
};

// 3)
// Канал с минимальным оставшимся временем обслуживания освободится раньше прихода очередной заявки
// Освобождается канал с минимальным оставшимся временем обслуживания.
static inline auto FreeMin(RequestsFlow& req_generator,
                           Request& next_request,
                           std::vector<Request>& channels,
                           RequestsQueue& queue,
                           std::pair<int, int>& system_status,
                           SimulationStatus& simulation_status,
                           std::vector<Request>::iterator free_it,
                           std::vector<Request>::iterator min_it,
                           std::function<void(const Request&)> write_request) -> Event
{
    simulation_status.time_passed = min_it->serve_end;

    // Изменение количества заявок в СМО
    (min_it->type == RequestType::kFirst) ? --system_status.first : --system_status.second;
    //Запись данных в новое событие
    Event current_event = {.number = simulation_status.event_number,
                           .time = simulation_status.time_passed,
                           .type = (min_it->type == RequestType::kFirst) ? 5 : 6,
                           .system_status = system_status,
                           .queue_status = queue.status(),
                           .request = min_it->number,
                           .time_next = next_request.arrive_time - simulation_status.time_passed};

    //(*min_it) - освободившаяся заявка
    //Только здесь изменяется число обработанных заявок
    if (min_it->type == RequestType::kFirst) {
        simulation_status.serve_total.first += min_it->serve_time;
        simulation_status.requests_served.first++;
        if (min_it->wait_time > 0) {
            simulation_status.wait_total.first += min_it->wait_time;
            simulation_status.requests_waited.first++;
        }
    } else {
        simulation_status.serve_total.second += min_it->serve_time;
        simulation_status.requests_served.second++;
        if (min_it->wait_time > 0) {
            simulation_status.wait_total.second += min_it->wait_time;
            simulation_status.requests_waited.second++;
        }
    }

    // Обработанная заявка отправляется в поток вывода
    if (write_request) {
        write_request(*min_it);
    }

    // Освобождение прибора с минимальным оставшимся временем обслуживания
    (*min_it) = Request{};

    return current_event;
};

// 4)
//  Очередная заявка прийдёт раньше, чем освободится канал с минимальным оставшимся временем обслуживания
//  Очередная заявка встаёт в очередь.
static inline auto EnqueueNext(RequestsFlow& req_generator,
                               Request& next_request,
                               std::vector<Request>& channels,
                               RequestsQueue& queue,
                               std::pair<int, int>& system_status,
                               SimulationStatus& simulation_status,
                               std::vector<Request>::iterator free_it,
                               std::vector<Request>::iterator min_it) -> Event
{
    simulation_status.time_passed = next_request.arrive_time;

    double prev_arrive_time = next_request.arrive_time;
    next_request.queue_number = queue.status().first
                                + (queue.status().second
                                   * (next_request.type == RequestType::kSecond))
                                + 1;
    queue.push(next_request);
    // Новая заявка в системе
    (next_request.type == RequestType::kFirst) ? ++simulation_status.requests_total.first
                                               : ++simulation_status.requests_total.second;

    next_request = req_generator(++simulation_status.request_number);
    //Запись данных в новое событие
    return {.number = simulation_status.event_number,
            .time = simulation_status.time_passed,
            .type = (next_request.type == RequestType::kFirst) ? 3 : 4,
            .system_status = system_status,
            .queue_status = queue.status(),
            .request = (next_request.number - 1),
            .time_next = next_request.arrive_time - prev_arrive_time};
};

SimulationResult Simulate(double lambda_th,
                          double mu_th,
                          int channels_number,
                          double prop,
                          int max_events,
                          std::function<void(const Event&)> write_event,
                          std::function<void(const Request&)> write_request)
{
    RequestsFlow req_generator(lambda_th, mu_th, prop);

    std::vector<Request> channels(channels_number);
    RequestsQueue queue{};
    std::pair<int, int> system_status{};

    SimulationStatus simulation_status{};

    // В систему поступила первая заявка. Фактически количество сгенерированных событий
    // Номер последнего события
    simulation_status.event_number = 1;

    // Очередная заявка
    Request next_request = req_generator(++simulation_status.request_number);

    for (; simulation_status.event_number <= max_events; ++simulation_status.event_number) {
        Event current_event{};
        auto [free_it, min_it] = FreeMin(channels);

        const auto prev_status = system_status;
        const auto prev_time = simulation_status.time_passed;

        if (IsEmpty(*min_it) || next_request.arrive_time < min_it->serve_end) {
            // Все приборы свободны или следующая заявка придёт раньше, чем освободится прибор
            if (free_it != end(channels)) {
                // Есть свободный прибор
                if (queue.empty()) {
                    // Очередь пуста
                    // Первый свободный прибор принимает очередную заявку (1)
                    current_event = StartNext(req_generator,
                                              next_request,
                                              channels,
                                              queue,
                                              system_status,
                                              simulation_status,
                                              free_it,
                                              min_it);
                } else {
                    // В очереди есть заявка
                    // Первый свободный прибор принимает заявку из очереди (2)
                    current_event = StartQueued(req_generator,
                                                next_request,
                                                channels,
                                                queue,
                                                system_status,
                                                simulation_status,
                                                free_it,
                                                min_it);
                }
            } else {
                // Все приборы заняты
                // Следующая заявка встаёт в очередь (4)
                current_event = EnqueueNext(req_generator,
                                            next_request,
                                            channels,
                                            queue,
                                            system_status,
                                            simulation_status,
                                            free_it,
                                            min_it);
            }
        } else {
            // Есть загруженный прибор и прибор освободится раньше, чем придёт следующая заявка
            if (free_it != end(channels)) {
                if (queue.empty()) {
                    // Очередь пуста
                    // Освобождается прибор с минимальным оставшимся временем обслуживания (3)
                    current_event = FreeMin(req_generator,
                                            next_request,
                                            channels,
                                            queue,
                                            system_status,
                                            simulation_status,
                                            free_it,
                                            min_it,
                                            write_request);
                } else {
                    // В очереди есть заявка
                    // Первый свободный прибор принимает заявку из очереди (2)
                    current_event = StartQueued(req_generator,
                                                next_request,
                                                channels,
                                                queue,
                                                system_status,
                                                simulation_status,
                                                free_it,
                                                min_it);
                }
            } else {
                // Все приборы заняты
                // Освобождается прибор с минимальным оставшимся временем обслуживания (3)
                current_event = FreeMin(req_generator,
                                        next_request,
                                        channels,
                                        queue,
                                        system_status,
                                        simulation_status,
                                        free_it,
                                        min_it,
                                        write_request);
            }
        }

        if (simulation_status.time_passed < prev_time) {
            std::cout << simulation_status.time_passed << '\t' << prev_time << '\n';
        }

        simulation_status.requests_weighted_summ.first += prev_status.first
                                                          * (simulation_status.time_passed
                                                             - prev_time);
        simulation_status.requests_weighted_summ.second += prev_status.second
                                                           * (simulation_status.time_passed
                                                              - prev_time);

        // Каждое ветвление порождает событие
        if (write_event) {
            write_event(current_event);
        }
    } //for

    if (write_request) {
        // Записываем оставшиеся в каналах заявки.
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

    // TODO: передавать avg_results
    return CalcResult(simulation_status);
}

} // namespace queueing_system
