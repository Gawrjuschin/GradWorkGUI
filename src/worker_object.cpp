#include "worker_object.h"
#include "graphs_data.h"
#include "input_data.h"
#include "progress.h"
#include "synchronizer.h"

#include <QueSys/queueing_system.h>

#include <QThread>

Worker_Object::Worker_Object(Synchronizer& synchronizer,
			     const InputData& input_data,
			     std::shared_ptr<Graphs_Data> gdata,
			     int thr_id,
			     QObject* parent)
    : QObject(parent)
    , r_synchronizer(synchronizer)
    , r_input_data(input_data)
    , p_gdata(gdata)
    , m_thr_id(thr_id)
{}

Worker_Object::~Worker_Object() = default;

void Worker_Object::process()
{
  auto lambda = InputData::min_load * r_input_data.mu * r_input_data.channels;
  m_counter = m_thr_id;
  constexpr double dL = (InputData::max_load - InputData::min_load) / Graphs_Data::POINTS_COUNT;

  //Шаг нагрузки. Остальное будет считаться через него
  auto& w_0 = p_gdata->data(0);
  auto& w_1 = p_gdata->data(1);
  auto& u_0 = p_gdata->data(2);
  auto& u_1 = p_gdata->data(3);
  auto& z_0 = p_gdata->data(4);
  auto& z_1 = p_gdata->data(5);

  auto& progress = r_synchronizer.getProgress();

  // Пересчитывает границы для графиков
  auto range_calc = [p_gdata = this->p_gdata](int point_index) {
    for (auto graph_index = 0; graph_index < Graphs_Data::GRAPHS_COUNT; ++graph_index) {
      auto point = p_gdata->data(graph_index)[point_index].y();
      QPointF& range = p_gdata->range(graph_index);
      if (point < range.x()) {
	range.rx() = point;
	continue;
      }
      if (point > range.y()) {
	range.ry() = point;
	continue;
      }
    }
  };

  // Прогоняю первый раз, чтобы заполнить пределы графов
  {
    auto sim_result = queueing_system::Simulate(lambda,
						r_input_data.mu,
						r_input_data.channels,
						r_input_data.propability,
						queueing_system::MaxEventsCondition(
						    r_input_data.events));

    w_0[m_counter] = {lambda, sim_result.avg_wait.first};
    w_1[m_counter] = {lambda, sim_result.avg_wait.second};
    u_0[m_counter] = {lambda, sim_result.avg_utility.first};
    u_1[m_counter] = {lambda, sim_result.avg_utility.second};
    z_0[m_counter] = {lambda, sim_result.avg_requests.first};
    z_1[m_counter] = {lambda, sim_result.avg_requests.second};

    m_counter += r_synchronizer.getThreadNum();
    ++r_synchronizer;
  }

  // Инициализирую минимум и максимум первой точкой
  for (auto graph_index = 0; graph_index < Graphs_Data::GRAPHS_COUNT; ++graph_index) {
    p_gdata->range(graph_index) = {p_gdata->data(graph_index).front().y(),
				   p_gdata->data(graph_index).front().y()};
  }

  while (m_counter < Graphs_Data::POINTS_COUNT && !r_synchronizer.canceled()) {
    r_synchronizer.sleep(); // Реализация паузы
    lambda = (InputData::min_load + dL * m_counter) * (r_input_data.mu * r_input_data.channels);

    auto sim_result = queueing_system::Simulate(lambda,
						r_input_data.mu,
						r_input_data.channels,
						r_input_data.propability,
						queueing_system::MaxEventsCondition(
						    r_input_data.events));

    w_0[m_counter] = {lambda, sim_result.avg_wait.first};
    w_1[m_counter] = {lambda, sim_result.avg_wait.second};
    u_0[m_counter] = {lambda, sim_result.avg_utility.first};
    u_1[m_counter] = {lambda, sim_result.avg_utility.second};
    z_0[m_counter] = {lambda, sim_result.avg_requests.first};
    z_1[m_counter] = {lambda, sim_result.avg_requests.second};

    range_calc(m_counter);

    m_counter += r_synchronizer.getThreadNum();
    ++progress;
  }

  if (++r_synchronizer >= r_synchronizer.getThreadNum()) {
    emit signal_done();
  }
  emit signal_finished();
}
