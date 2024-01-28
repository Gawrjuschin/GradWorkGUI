#include "graphs_data.h"
#include "QueSys/queueing_system.h"

#include <QLineSeries>

#include <QVector>

#include <array>
#include <cmath>

Graphs_Data::Graphs_Data()
{
  construct_series();
}

Graphs_Data::~Graphs_Data() = default;

void Graphs_Data::construct_series()
{
  static QPen pen_exp(Qt::blue);
  static QPen pen_apr(Qt::darkGray);
  pen_exp.setWidth(2);
  pen_apr.setWidth(2);
  pen_apr.setStyle(Qt::PenStyle::DotLine);
  for (std::size_t graph_number{}; graph_number < GRAPHS_COUNT; ++graph_number) {
    m_points_exp[graph_number] = new QtCharts::QLineSeries;
    m_points_apr[graph_number] = new QtCharts::QLineSeries;
    series(graph_number)->setPen(pen_exp);
    series_apr(graph_number)->setPen(pen_apr);
    m_exp_data[graph_number].resize(POINTS_COUNT);
    m_apr_data[graph_number].resize(POINTS_COUNT);
  }

  constexpr double dLoad = static_cast<double>(max_load - min_load) / POINTS_COUNT;
  for (std::size_t graph_number{}; graph_number < GRAPHS_COUNT; ++graph_number) {
    for (int point_number{}; point_number < POINTS_COUNT; ++point_number) {
      m_exp_data[graph_number][point_number].setX(dLoad * point_number);
      m_apr_data[graph_number][point_number].setX(dLoad * point_number);
    }
  }
}

QtCharts::QLineSeries* Graphs_Data::series(int i) noexcept
{
  return m_points_exp[i];
}

QtCharts::QLineSeries* Graphs_Data::series_apr(int i) noexcept
{
  return m_points_apr[i];
}

void Graphs_Data::load(int point, const queueing_system::SimulationResult& result) noexcept
{
  m_exp_data[W_0][point].setY(result.avg_wait.first);
  m_exp_data[W_1][point].setY(result.avg_wait.second);
  m_exp_data[U_0][point].setY(result.avg_utility.first);
  m_exp_data[U_1][point].setY(result.avg_utility.second);
  m_exp_data[Z_0][point].setY(result.avg_requests.first);
  m_exp_data[Z_1][point].setY(result.avg_requests.second);
}

void Graphs_Data::calc_Yranges() noexcept
{
  for (std::size_t graph_number{}; graph_number < GRAPHS_COUNT; ++graph_number) {
    auto [min, max] = std::minmax_element(m_exp_data[graph_number].cbegin(),
					  m_exp_data[graph_number].cend(),
					  [](const QPointF& lhs, const QPointF& rhs) -> bool {
					    return lhs.y() < rhs.y();
					  });
    m_Yranges[graph_number] = {min->y(), max->y()};
  }
}

QVector<QPointF>& Graphs_Data::data(int i) noexcept
{
  return m_exp_data[i];
}

QVector<QPointF>& Graphs_Data::data_apr(int i) noexcept
{
  return m_apr_data[i];
}

QPointF& Graphs_Data::range(int i) noexcept
{
  return m_Yranges[i];
}

void Graphs_Data::update()
{
  for (auto i = 0; i < m_exp_data.size(); ++i) {
    m_points_exp[i]->replace(m_exp_data[i]);
  }
}

void Graphs_Data::update_apr(int i)
{
  m_points_apr[i]->replace(m_apr_data[i]);
}

void Graphs_Data::approximate(int index)
{ // Применяется МНК к указанному графику
  constexpr int N = Graphs_Data::POINTS_COUNT;
  double sx = 0;
  double sx2 = 0;
  double sy = 0;
  double sxy = 0;
  auto& points = data(index);
  auto& apr = data_apr(index);
  for (auto& pnt : points) {
    sx += pnt.x();
    sx2 += pnt.x() * pnt.x();
    sy += log(pnt.y());
    sxy += pnt.x() * log(pnt.y());
  }
  auto b = (N * sxy - sx * sy) / (N * sx2 - sx * sx);
  auto a = exp((sy - b * sx) / N);
  for (auto i = 0; i < N; ++i) {
    apr[i] = QPointF{points[i].x(), a * exp(b * points[i].x())};
  }
  update_apr(index);
}
