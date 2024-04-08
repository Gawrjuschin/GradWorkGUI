#ifndef GRAPHS_DATA_H
#define GRAPHS_DATA_H

#include <QPointF>
#include <QVector>
#include <array>

#include <QueSys/queueing_system.h>
#include <cmath>

class PointsData
{
public:
  static constexpr std::size_t kPointsCount = 1'000;
  static constexpr std::size_t kGraphsCount = 6;

  // Вместо int
  enum Graph : int {
    kWaitFirst,
    kWaitSecond,
    kUtilityFirst,
    kUtilitySecond,
    kAVGFirst,
    kAVGSecond
  };

  PointsData()
      : m_experimantal{QVector<QPointF>(kPointsCount),
                       QVector<QPointF>(kPointsCount),
                       QVector<QPointF>(kPointsCount),
                       QVector<QPointF>(kPointsCount),
                       QVector<QPointF>(kPointsCount),
                       QVector<QPointF>(kPointsCount)},
        m_approximation{
            QVector<QPointF>(kPointsCount), QVector<QPointF>(kPointsCount),
            QVector<QPointF>(kPointsCount), QVector<QPointF>(kPointsCount),
            QVector<QPointF>(kPointsCount), QVector<QPointF>(kPointsCount)} {
    // Выделяю память для точек
    std::for_each(
        begin(m_experimantal), end(m_experimantal),
        [](QVector<QPointF>& points) { points.resize(kPointsCount); });
    std::for_each(
        begin(m_approximation), end(m_approximation),
        [](QVector<QPointF>& points) { points.resize(kPointsCount); });
  }

  void load_result(int index,
                   const queueing_system::SimulationResult& result) noexcept {
    m_experimantal[kWaitFirst][index].setY(result.avg_wait.first);
    m_experimantal[kWaitSecond][index].setY(result.avg_wait.second);
    m_experimantal[kUtilityFirst][index].setY(result.avg_utility.first);
    m_experimantal[kUtilitySecond][index].setY(result.avg_utility.second);
    m_experimantal[kAVGFirst][index].setY(result.avg_requests.first);
    m_experimantal[kAVGSecond][index].setY(result.avg_requests.second);
  }

  void process_results() {
    for (int graph = 0; graph < kGraphsCount; ++graph) {
      calc_ranges(graph);
      approximate(graph);
    }
  }

  const QVector<QPointF>& pointsExperimental(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_experimantal.at(graph);
  }

  const QVector<QPointF>& pointsApproximation(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_approximation.at(graph);
  }

  QPointF RangeX(int graph) const noexcept
  {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    const auto& points = m_experimantal[graph];
    return {points.front().x(), points.back().x()};
  }
  QPointF RangeY(int graph) const noexcept
  {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_y_ranges[graph];
  }

private:
  void calc_ranges(int graph) {
    const auto [max, min] = std::minmax_element(
        std::cbegin(m_experimantal[graph]), std::cend(m_experimantal[graph]),
        [](const QPointF& lhs, const QPointF& rhs) {
          return lhs.x() < rhs.x();
        });
    m_y_ranges.at(graph) = QPointF{min->x(), max->x()};
  }

  void approximate(const int graph) { // Применяется МНК к указанному графику
    constexpr int N = kPointsCount;
    double sx = 0;
    double sx2 = 0;
    double sy = 0;
    double sxy = 0;
    for (auto& point : m_experimantal[graph]) {
      sx += point.x();
      sx2 += point.x() * point.x();
      sy += std::log(point.y());
      sxy += point.x() * std::log(point.y());
    }
    auto b = (N * sxy - sx * sy) / (N * sx2 - sx * sx);
    auto a = std::exp((sy - b * sx) / N);
    for (auto index = 0; index < N; ++index) {
      m_approximation[graph][index] =
          QPointF{m_experimantal[graph][index].x(),
                  a * exp(b * m_experimantal[graph][index].x())};
    }
  }

private:
  std::array<QVector<QPointF>, kPointsCount> m_experimantal;
  std::array<QVector<QPointF>, kPointsCount> m_approximation;

  std::array<QPointF, kGraphsCount> m_y_ranges{};
};

namespace QtCharts {
class QLineSeries;
}

class Graphs_Data
{
public:
  static constexpr double min_load = 0.35;
  static constexpr double max_load = 0.95;
  static constexpr long W_0 = 0;
  static constexpr long W_1 = 0;
  static constexpr long U_0 = 0;
  static constexpr long U_1 = 0;
  static constexpr long Z_0 = 0;
  static constexpr long Z_1 = 0;
  static constexpr long GRAPHS_COUNT = 6;
  static constexpr long POINTS_COUNT = 1000;

  Graphs_Data();
  ~Graphs_Data();

  QtCharts::QLineSeries* series(const int i) noexcept;
  QtCharts::QLineSeries* series_apr(int i) noexcept;

  void load(int point,
            const queueing_system::SimulationResult& result) noexcept {
    m_exp_data[W_0][point].setY(result.avg_wait.first);
    m_exp_data[W_1][point].setY(result.avg_wait.second);
    m_exp_data[U_0][point].setY(result.avg_utility.first);
    m_exp_data[U_1][point].setY(result.avg_utility.second);
    m_exp_data[Z_0][point].setY(result.avg_requests.first);
    m_exp_data[Z_1][point].setY(result.avg_requests.second);
  }

  void calc_Yranges() noexcept;

  QVector<QPointF>& data(int i) noexcept;
  QVector<QPointF>& data_apr(int i) noexcept;

  QPointF& range(int i) noexcept;

  void update();
  void update_apr(int i);

  void approximate(int index);

private:
  std::array<QtCharts::QLineSeries*, GRAPHS_COUNT> m_points_exp{};
  std::array<QtCharts::QLineSeries*, GRAPHS_COUNT> m_points_apr{};

  std::array<QVector<QPointF>, GRAPHS_COUNT> m_exp_data{};
  std::array<QVector<QPointF>, GRAPHS_COUNT> m_apr_data{};

  // Это по оси Y. Ось X известна
  std::array<QPointF, GRAPHS_COUNT> m_Yranges{};

private:
  void construct_series();
};

#endif // GRAPHS_DATA_H
