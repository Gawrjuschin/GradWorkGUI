#include "queueing_system.h"
#include "requests_flow.hpp"
#include "requests_queue.hpp"

#include <cassert>
#include <string>

namespace queueing_system {

namespace detail {
static inline auto FreeMin(std::vector<Request>& channels) noexcept {
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
  return std::make_pair(free, min);
}

} // namespace detail

SimulationResult
CalcResult(const SimulationStatus& simulation_status) noexcept {
  SimulationResult result{};

  const auto [request_number, event_number, time_passed, serve_total,
              wait_total, requests_total, requests_served, requests_waited,
              requests_weighted_summ] = simulation_status;

  result.time_passed = time_passed;
  result.requests = request_number;
  result.events = event_number;
  result.avg_period = {time_passed / requests_total.first,
                       time_passed / requests_total.second};
  result.avg_serve = {serve_total.first / requests_served.first,
                      serve_total.second / requests_served.second};
  result.propability = {
      requests_total.first / (requests_total.first + requests_total.second),
      requests_total.second / (requests_total.first + requests_total.second)};

  // TODO: придумать лучшее решение для случаев, когда ожидания не было вообще
  if (requests_waited.second != 0) {
    result.avg_wait = {wait_total.first / requests_waited.first,
                       wait_total.second / requests_waited.second};
  }
  result.avg_utility = {result.avg_wait.first + result.avg_serve.first,
                        result.avg_wait.second + result.avg_serve.second};

  result.avg_requests = {requests_weighted_summ.first / time_passed,
                         requests_weighted_summ.second / time_passed};

  return result;
}

// 1) Очередная заявка встаёт на обслуживание. Первый свободный прибор принимает
// заявку
static inline auto StartNext(RequestsFlow& req_generator, Request& next_request,
                             std::vector<Request>& channels,
                             RequestsQueue& queue,
                             std::pair<int, int>& system_status,
                             SimulationStatus& simulation_status,
                             std::vector<Request>::iterator free_it) -> Event {
  simulation_status.time_passed = next_request.arrive_time;

  *free_it = next_request;
  // Новая заявка в системе
  (next_request.type == RequestType::kFirst)
      ? ++simulation_status.requests_total.first
      : ++simulation_status.requests_total.second;

  next_request = req_generator(++simulation_status.request_number);
  free_it->queue_number = 0;
  free_it->ch_number = std::distance(begin(channels), free_it) + 1;
  free_it->start_time = free_it->arrive_time;
  free_it->serve_end = free_it->arrive_time + free_it->serve_time;
  free_it->wait_time = 0;

  // Изменение количества заявок в СМО
  // Заменить тип на енам
  (free_it->type == RequestType::kFirst) ? ++system_status.first
                                         : ++system_status.second;

  return {.number = simulation_status.event_number,
          .time = simulation_status.time_passed,
          .type = (free_it->type == RequestType::kFirst) ? 1u : 2u,
          .system_status = system_status,
          .queue_status = queue.status(),
          .request = free_it->number,
          .time_next = next_request.arrive_time - free_it->arrive_time};
}

// 2)
// Первая в очереди заявка встаёт на обслуживание
// Первый свободный прибор принимает заявку
static inline Event StartQueued(Request& next_request,
                                std::vector<Request>& channels,
                                RequestsQueue& queue,
                                std::pair<int, int>& system_status,
                                SimulationStatus& simulation_status,
                                std::vector<Request>::iterator free_it) {
  (*free_it) = queue.front();

  // Изменение количества заявок в СМО
  (free_it->type == RequestType::kFirst) ? ++system_status.first
                                         : ++system_status.second;
  queue.pop();
  free_it->ch_number = std::distance(begin(channels), free_it) + 1;
  free_it->start_time = simulation_status.time_passed;
  free_it->wait_time = simulation_status.time_passed - free_it->arrive_time;
  free_it->serve_end = simulation_status.time_passed + free_it->serve_time;

  // Запись данных в новое событие
  return {.number = simulation_status.event_number,
          .time = simulation_status.time_passed,
          .type = (free_it->type == RequestType::kFirst) ? 7u : 8u,
          .system_status = system_status,
          .queue_status = queue.status(),
          .request = free_it->number,
          .time_next =
              next_request.arrive_time - simulation_status.time_passed};
}

// 3)
// Канал с минимальным оставшимся временем обслуживания освободится раньше
// прихода очередной заявки Освобождается канал с минимальным оставшимся
// временем обслуживания.
static inline Event
FreeMin(Request& next_request, RequestsQueue& queue,
        std::pair<int, int>& system_status, SimulationStatus& simulation_status,
        std::vector<Request>::iterator min_it,
        const std::function<void(const Request&)>& write_request) {
  simulation_status.time_passed = min_it->serve_end;

  // Изменение количества заявок в СМО
  (min_it->type == RequestType::kFirst) ? --system_status.first
                                        : --system_status.second;
  // Запись данных в новое событие
  Event current_event = {
      .number = simulation_status.event_number,
      .time = simulation_status.time_passed,
      .type = (min_it->type == RequestType::kFirst) ? 5u : 6u,
      .system_status = system_status,
      .queue_status = queue.status(),
      .request = min_it->number,
      .time_next = next_request.arrive_time - simulation_status.time_passed};

  //(*min_it) - освободившаяся заявка
  // Только здесь изменяется число обработанных заявок
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
}

// Перегрузка без записи вышедшей из системы заявки
static inline auto FreeMin(Request& next_request, RequestsQueue& queue,
                           std::pair<int, int>& system_status,
                           SimulationStatus& simulation_status,
                           std::vector<Request>::iterator min_it) -> Event {
  simulation_status.time_passed = min_it->serve_end;

  // Изменение количества заявок в СМО
  (min_it->type == RequestType::kFirst) ? --system_status.first
                                        : --system_status.second;
  // Запись данных в новое событие
  Event current_event = {
      .number = simulation_status.event_number,
      .time = simulation_status.time_passed,
      .type = (min_it->type == RequestType::kFirst) ? 5u : 6u,
      .system_status = system_status,
      .queue_status = queue.status(),
      .request = min_it->number,
      .time_next = next_request.arrive_time - simulation_status.time_passed};

  //(*min_it) - освободившаяся заявка
  // Только здесь изменяется число обработанных заявок
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

  // Освобождение прибора с минимальным оставшимся временем обслуживания
  (*min_it) = Request{};

  return current_event;
}

// 4)
//  Очередная заявка прийдёт раньше, чем освободится канал с минимальным
//  оставшимся временем обслуживания Очередная заявка встаёт в очередь.
static inline auto EnqueueNext(RequestsFlow& req_generator,
                               Request& next_request, RequestsQueue& queue,
                               std::pair<int, int>& system_status,
                               SimulationStatus& simulation_status) -> Event {
  simulation_status.time_passed = next_request.arrive_time;

  double prev_arrive_time = next_request.arrive_time;
  next_request.queue_number =
      queue.status().first +
      (queue.status().second * (next_request.type == RequestType::kSecond)) + 1;
  queue.push(next_request);
  // Новая заявка в системе
  (next_request.type == RequestType::kFirst)
      ? ++simulation_status.requests_total.first
      : ++simulation_status.requests_total.second;

  next_request = req_generator(++simulation_status.request_number);
  // Запись данных в новое событие
  return {.number = simulation_status.event_number,
          .time = simulation_status.time_passed,
          .type = (next_request.type == RequestType::kFirst) ? 3u : 4u,
          .system_status = system_status,
          .queue_status = queue.status(),
          .request = (next_request.number - 1),
          .time_next = next_request.arrive_time - prev_arrive_time};
};

SimulationResult
Simulate(double lambda_th, double mu_th, int channels_number, double prop,
         std::function<bool(const SimulationStatus&)> continue_condition,
         std::function<void(const Event&)> write_event,
         std::function<void(const Request&)> write_request) {
  assert(write_event != nullptr);
  assert(write_request != nullptr);

  RequestsFlow req_gen(lambda_th, mu_th, prop);
  std::vector<Request> channels(channels_number);
  RequestsQueue queue{};
  std::pair<int, int> sys_status{};
  SimulationStatus sim_status{};
  Request next_req = req_gen(++sim_status.request_number);

  // В систему поступила первая заявка. Фактически количество сгенерированных
  // событий Номер последнего события
  for (sim_status.event_number = 1; continue_condition(sim_status);
       ++sim_status.event_number) {
    Event current_event{};
    auto [free_it, min_it] = detail::FreeMin(channels);
    const auto prev_status = sys_status;
    const auto prev_time = sim_status.time_passed;

    // Все приборы свободны или следующая заявка придёт раньше, чем
    // освободится прибор
    if (IsEmpty(*min_it) || next_req.arrive_time < min_it->serve_end) {
      if (free_it != std::end(channels)) {
        // Есть свободный прибор
        if (std::empty(queue)) {
          // Очередь пуста.
          // Первый свободный прибор принимает очередную заявку (1)
          current_event = StartNext(req_gen, next_req, channels, queue,
                                    sys_status, sim_status, free_it);
        } else {
          // В очереди есть заявка
          // Первый свободный прибор принимает заявку из очереди (2)
          current_event = StartQueued(next_req, channels, queue, sys_status,
                                      sim_status, free_it);
        }
      } else {
        // Все приборы заняты
        // Следующая заявка встаёт в очередь (4)
        current_event =
            EnqueueNext(req_gen, next_req, queue, sys_status, sim_status);
      }
    } else {
      // Есть загруженный прибор и прибор освободится раньше, чем придёт
      // следующая заявка
      if (free_it != end(channels)) {
        if (queue.empty()) {
          // Очередь пуста
          // Освобождается прибор с минимальным оставшимся временем обслуживания
          // (3)
          current_event = FreeMin(next_req, queue, sys_status, sim_status,
                                  min_it, write_request);
        } else {
          // В очереди есть заявка
          // Первый свободный прибор принимает заявку из очереди (2)
          current_event = StartQueued(next_req, channels, queue, sys_status,
                                      sim_status, free_it);
        }
      } else {
        // Все приборы заняты
        // Освобождается прибор с минимальным оставшимся временем обслуживания
        // (3)
        current_event = FreeMin(next_req, queue, sys_status, sim_status, min_it,
                                write_request);
      }
    }

    sim_status.requests_weighted_summ.first +=
        prev_status.first * (sim_status.time_passed - prev_time);
    sim_status.requests_weighted_summ.second +=
        prev_status.second * (sim_status.time_passed - prev_time);

    // Каждое ветвление порождает событие
    if (write_event) {
      write_event(current_event);
    }
  } // for

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

  return CalcResult(sim_status);
}

SimulationResult
Simulate(double lambda_th, double mu_th, int channels_number, double prop,
         std::function<bool(const SimulationStatus&)> continue_condition) {
  RequestsFlow req_gen(lambda_th, mu_th, prop);
  std::vector<Request> channels(channels_number);
  RequestsQueue queue{};
  std::pair<int, int> sys_status{};
  SimulationStatus sim_status{};
  Request next_req = req_gen(++sim_status.request_number);

  // В систему поступила первая заявка. Фактически количество сгенерированных
  // событий Номер последнего события
  for (sim_status.event_number = 1; continue_condition(sim_status);
       ++sim_status.event_number) {
    Event current_event{};
    auto [free_it, min_it] = detail::FreeMin(channels);
    const auto prev_status = sys_status;
    const auto prev_time = sim_status.time_passed;

    // Все приборы свободны или следующая заявка придёт раньше, чем
    // освободится прибор
    if (IsEmpty(*min_it) || next_req.arrive_time < min_it->serve_end) {
      if (free_it != std::end(channels)) {
        // Есть свободный прибор
        if (std::empty(queue)) {
          // Очередь пуста.
          // Первый свободный прибор принимает очередную заявку (1)
          current_event = StartNext(req_gen, next_req, channels, queue,
                                    sys_status, sim_status, free_it);
        } else {
          // В очереди есть заявка
          // Первый свободный прибор принимает заявку из очереди (2)
          current_event = StartQueued(next_req, channels, queue, sys_status,
                                      sim_status, free_it);
        }
      } else {
        // Все приборы заняты
        // Следующая заявка встаёт в очередь (4)
        current_event =
            EnqueueNext(req_gen, next_req, queue, sys_status, sim_status);
      }
    } else {
      // Есть загруженный прибор и прибор освободится раньше, чем придёт
      // следующая заявка
      if (free_it != end(channels)) {
        if (queue.empty()) {
          // Очередь пуста
          // Освобождается прибор с минимальным оставшимся временем обслуживания
          // (3)
          current_event =
              FreeMin(next_req, queue, sys_status, sim_status, min_it);
        } else {
          // В очереди есть заявка
          // Первый свободный прибор принимает заявку из очереди (2)
          current_event = StartQueued(next_req, channels, queue, sys_status,
                                      sim_status, free_it);
        }
      } else {
        // Все приборы заняты
        // Освобождается прибор с минимальным оставшимся временем обслуживания
        // (3)
        current_event =
            FreeMin(next_req, queue, sys_status, sim_status, min_it);
      }
    }

    sim_status.requests_weighted_summ.first +=
        prev_status.first * (sim_status.time_passed - prev_time);
    sim_status.requests_weighted_summ.second +=
        prev_status.second * (sim_status.time_passed - prev_time);

    // Каждое ветвление порождает событие

  } // for

  return CalcResult(sim_status);
}

} // namespace queueing_system
