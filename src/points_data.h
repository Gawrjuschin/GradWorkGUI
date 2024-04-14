#ifndef POINTS_DATA_H
#define POINTS_DATA_H

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
  static constexpr double kMinLoad = 0.35;
  static constexpr double kMaxLoad = 0.95;

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

  void load_result(int index, const qreal load,
                   const queueing_system::SimulationResult& result) noexcept {
    m_experimantal[kWaitFirst][index] = {load, result.avg_wait.first};
    m_experimantal[kWaitSecond][index] = {load, result.avg_wait.second};
    m_experimantal[kUtilityFirst][index] = {load, result.avg_utility.first};
    m_experimantal[kUtilitySecond][index] = {load, result.avg_utility.second};
    m_experimantal[kAVGFirst][index] = {load, result.avg_requests.first};
    m_experimantal[kAVGSecond][index] = {load, result.avg_requests.second};
  }

  void process_results() {
    for (int graph = 0; graph < kGraphsCount; ++graph) {
      calc_y_ranges(graph);
      aproximate(graph);
    }
  }

  const QVector<QPointF>& pointsExperimental(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_experimantal.at(graph);
  }

  const QVector<QPointF>& pointsAproximation(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_approximation.at(graph);
  }
  constexpr std::pair<qreal, qreal> RangeX(int graph) const noexcept {
    return {PointsData::kMinLoad, PointsData::kMaxLoad};
  }

  std::pair<qreal, qreal> RangeY(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_y_ranges[graph];
  }

private:
  void calc_y_ranges(int graph) {
    const auto [min, max] = std::minmax_element(
        std::cbegin(m_experimantal[graph]), std::cend(m_experimantal[graph]),
        [](const QPointF& lhs, const QPointF& rhs) {
          return lhs.y() < rhs.y();
        });
    m_y_ranges[graph] = {min->y(), max->y()};
  }

  void aproximate(const int graph) { // Применяется МНК к указанному графику
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

  std::array<std::pair<qreal, qreal>, kGraphsCount> m_y_ranges{};
};

#endif // POINTS_DATA_H
