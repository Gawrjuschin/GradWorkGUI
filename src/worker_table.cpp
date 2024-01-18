#include "worker_table.h"
#include "input_data.h"
#include "models.h"
#include "synchronizer.h"
#include "table_data.h"

#include <QueSys/queueing_system.h>

#include <QThread>
#include <memory>

Worker_Table::Worker_Table(const Synchronizer& synchronizer,
                           const InputData& input_data,
                           std::shared_ptr<Table_Data> table_data,
                           QObject* parent)
    : QObject(parent)
    , r_synchronizer(synchronizer)
    , r_input_data(input_data)
    , p_tdata(table_data)
{}

Worker_Table::~Worker_Table() = default;

void Worker_Table::process()
{
    const auto lambda = InputData::max_load * r_input_data.mu * r_input_data.channels;

    QVector<Request> requests;
    QVector<Event> events;
    // В ненагруженной системе заявки не встают в очередь (только два события на заявку)
    requests.reserve(r_input_data.events / 2);
    events.reserve(r_input_data.events);

    [[maybe_unused]] auto sim_result = queueing_system::Simulate(
        lambda,
        r_input_data.mu,
        r_input_data.channels,
        r_input_data.propability,
        queueing_system::MaxEventsCondition(r_input_data.events),
        [&events](const Event& event) { events.push_back(event); },
        [&requests](const Request& request) { requests.push_back(request); });

    p_tdata->event_model()->append(std::move(events));
    p_tdata->request_model()->append(std::move(requests));

    if (!r_synchronizer.canceled()) {
        emit signal_finished();
    }
}
