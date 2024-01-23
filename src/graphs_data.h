#ifndef GRAPHS_DATA_H
#define GRAPHS_DATA_H

#include <QPointF>
#include <QVector>
#include <array>

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
    kUtilitySecont,
    kAVGFirst, // number of requests (L from Little)
    kAVGSecond
  };

  PointsData()
      : m_points_experimantal{QVector<QPointF>(kPointsCount),
			      QVector<QPointF>(kPointsCount),
			      QVector<QPointF>(kPointsCount),
			      QVector<QPointF>(kPointsCount),
			      QVector<QPointF>(kPointsCount),
			      QVector<QPointF>(kPointsCount)}
      , m_points_approximation{QVector<QPointF>(kPointsCount),
			       QVector<QPointF>(kPointsCount),
			       QVector<QPointF>(kPointsCount),
			       QVector<QPointF>(kPointsCount),
			       QVector<QPointF>(kPointsCount),
			       QVector<QPointF>(kPointsCount)}
  {}

  void ReplacePoints(int graph, QVector<QPointF>&& points)
  {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    m_points_experimantal.at(graph).swap(points);

    // TODO: расчёт аппроксимации
    {
      std::copy(std::begin(m_points_experimantal.at(graph)),
		std::end(m_points_experimantal.at(graph)),
		std::begin(m_points_approximation.at(graph)));
    }

    // Расчёт границ
    {
      const auto [max, min] = std::minmax_element(std::cbegin(m_points_experimantal.at(graph)),
						  std::cend(m_points_experimantal.at(graph)),
						  [](const QPointF& lhs, const QPointF& rhs) {
						    return lhs.x() < rhs.x();
						  });
      m_y_ranges.at(graph) = QPointF{min->x(), max->x()};
    }
  }

  const QVector<QPointF>& PointsExperimental(int graph) const noexcept
  {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_points_experimantal.at(graph);
  }

  const QVector<QPointF>& PointsApproximation(int graph) const noexcept
  {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_points_approximation.at(graph);
  }

  QPointF RangeX(int graph) const noexcept
  {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    const auto& points = m_points_experimantal.at(graph / 2);
    return {points.front().x(), points.back().x()};
  }
  QPointF RangeY(int graph) const noexcept
  {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_y_ranges.at(graph / 2);
  }

private:
  std::array<QVector<QPointF>, kGraphsCount> m_points_experimantal;
  std::array<QVector<QPointF>, kPointsCount> m_points_approximation;

  std::array<QPointF, kPointsCount / 2> m_y_ranges{};
};

namespace QtCharts {
class QLineSeries;
}

class Graphs_Data
{
public:
  enum { W_0 = 0, W_1, U_0, U_1, Z_0, Z_1, GRAPHS_COUNT = 6, POINTS_COUNT = 1000 };

  Graphs_Data();
  ~Graphs_Data();

  QtCharts::QLineSeries* series(int i);
  QtCharts::QLineSeries* series_apr(int i);

  QVector<QPointF>& data(int i);
  QVector<QPointF>& data_apr(int i);

  QPointF& range(int i);

  void update();
  void update_apr(int i);

  void approximate(int index);

private:
  std::array<QtCharts::QLineSeries*, GRAPHS_COUNT> m_points_exp;
  std::array<QtCharts::QLineSeries*, GRAPHS_COUNT> m_points_apr;

  QVector<QVector<QPointF>> m_exp_data;
  QVector<QVector<QPointF>> m_apr_data;
  std::array<QPointF, GRAPHS_COUNT> m_ranges;

private:
  void construct_series();
};

#endif // GRAPHS_DATA_H
