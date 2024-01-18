#include "queueing_system.h"

#include "requests_flow.h"
#include "requests_queue.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include <cassert>
#include <iostream>

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

SimulationResult CalcResult(const SimulationStatus& simulation_status) noexcept
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
                          std::function<bool(const SimulationStatus&)> continue_condition,
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

    for (; continue_condition(simulation_status); ++simulation_status.event_number) {
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
