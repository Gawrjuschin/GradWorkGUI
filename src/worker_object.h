#ifndef WORKER_OBJECT_H
#define WORKER_OBJECT_H

// #include "QueSys/queueing_system.h"
// #include "graphs_data.h"
// #include "input_data.h"
// #include "synchronizer.h"
// #include "threadscontrol.h"

// #include <memory>

// inline auto MakeWorkPartObject(const ThreadsControl& threads_control,
// 			       Progress& progress,
// 			       const InputData& input_data,
// 			       Graphs_Data& graphs_data,
// 			       const std::pair<std::size_t, std::size_t>& points_range)

// {
//   return [&threads_control, &progress, ] {}
// }

// inline auto GetWorkerObjectFunction(Synchronizer& synchronizer,
// 				    const InputData& input_data,
// 				    std::shared_ptr<Graphs_Data> gdata,
// 				    int thr_id)
// {
//   return [&synchronizer, &input_data, gdata, thr_id]() {
//     auto counter = 0;
//     //Шаг нагрузки. Остальное будет считаться через него
//     auto& w_0 = gdata->data(Graphs_Data::W_0);
//     auto& w_1 = gdata->data(Graphs_Data::W_1);
//     auto& u_0 = gdata->data(Graphs_Data::U_0);
//     auto& u_1 = gdata->data(Graphs_Data::U_1);
//     auto& z_0 = gdata->data(Graphs_Data::Z_0);
//     auto& z_1 = gdata->data(Graphs_Data::Z_1);

//     auto& progress = synchronizer.getProgress();

//     // Инициализирую минимум и максимум первой точкой
//     for (auto i = 0; i < Graphs_Data::GRAPHS_COUNT; ++i) {
//       gdata->range(i) = {gdata->data(i).front().y(), gdata->data(i).front().y()};
//     }
//     counter = thr_id;
//     constexpr double dL = (InputData::max_load - InputData::min_load)
// 			  / static_cast<long>(Graphs_Data::POINTS_COUNT);

//     while (!synchronizer.canceled() && counter < Graphs_Data::POINTS_COUNT) {
//       synchronizer.sleep(); // Реализация паузы

//       const auto lambda = (InputData::min_load + dL * counter)
// 			  * (input_data.mu * input_data.channels);

//       const auto sim_result = queueing_system::Simulate(lambda,
// 							input_data.mu,
// 							input_data.channels,
// 							input_data.propability,
// 							queueing_system::MaxEventsCondition(
// 							    input_data.events));

//       Q_ASSERT(0 <= counter && counter < w_0.size());
//       w_0[counter] = {lambda, sim_result.avg_wait.first};
//       w_1[counter] = {lambda, sim_result.avg_wait.second};
//       u_0[counter] = {lambda, sim_result.avg_utility.first};
//       u_1[counter] = {lambda, sim_result.avg_utility.second};
//       z_0[counter] = {lambda, sim_result.avg_requests.first};
//       z_1[counter] = {lambda, sim_result.avg_requests.second};

//       counter += synchronizer.getThreadNum();
//       ++progress;
//     }

//     // TODO: убрать барьер
//     ++synchronizer;
//   };
// }

#endif // WORKER_OBJECT_H
