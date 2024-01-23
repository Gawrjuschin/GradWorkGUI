#ifndef WORKER_TABLE_H
#define WORKER_TABLE_H

#include <QueSys/queueing_system.h>
#include <QueSys/request.h>
#include "input_data.h"
#include "models.h"
#include "table_data.h"
#include <memory>

class Synchronizer;
class Table_Data;
struct InputData;

inline auto GetWorkerTableFunction(const Synchronizer& synchronizer,
				   const InputData& input_data,
				   std::shared_ptr<Table_Data> table_data)
{
  return [&input_data, table_data]() {
    constexpr int events_limit = 1'000;
    constexpr int requests_limit = events_limit / 2;

    QVector<Event> generated_events{};
    generated_events.reserve(events_limit);
    QVector<Request> generated_requests{};
    generated_requests.reserve(requests_limit);

    auto lambda = InputData::max_load * input_data.mu * input_data.channels;

    [[maybe_unused]] auto res = queueing_system::Simulate(
	lambda,
	input_data.mu,
	input_data.channels,
	input_data.propability,
	queueing_system::MaxEventsCondition(input_data.events),
	[&generated_events](const Event& event) { generated_events.push_back(event); },
	[&generated_requests](const Request& request) { generated_requests.push_back(request); });

    table_data->event_model()->clear();
    table_data->request_model()->clear();

    table_data->event_model()->append(std::move(generated_events));
    table_data->request_model()->append(std::move(generated_requests));
  };
}

#endif // WORKER_TABLE_H
