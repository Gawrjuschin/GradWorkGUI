#ifndef POINTS_DATA_H
#define POINTS_DATA_H

#include <QPointF>
#include <QVector>
#include <array>

#include <QueSys/queueing_system.h>
#include <cmath>

/**
 * @brief The PointsData class - отвечает за хранение результатов моделирования
 * в виде 6 векторов точек. Вычисляет границы и аппроксимацию графиков.
 */
class PointsData {

public:
  /**
   * @brief The Graph enum - индексы графиков, соответствующие характеристикам
   * СМО.
   */
  enum Graph : int {
    kWaitFirst,
    kWaitSecond,
    kUtilityFirst,
    kUtilitySecond,
    kAVGFirst,
    kAVGSecond
  };

  /**
   * @brief kPointsCount - количество точек в графике.
   */
  static constexpr int kPointsCount = 1'000;
  /**
   * @brief kGraphsCount - количество графиков.
   */
  static constexpr int kGraphsCount = 6;
  /**
   * @brief kMinLoad - минимальный коэффициент нагрузки СМО.
   */
  static constexpr double kMinLoad = 0.35;
  /**
   * @brief kMaxLoad - максимальный коэффициент нагрузки СМО.
   */
  static constexpr double kMaxLoad = 0.95;

  std::array<QVector<QPointF>, kPointsCount> m_experimantal{};
  std::array<QVector<QPointF>, kPointsCount> m_approximation{};
  std::array<std::pair<qreal, qreal>, kGraphsCount> m_y_ranges{};

public:
  PointsData() {
    for (std::size_t idx{}; idx < kGraphsCount; ++idx) {
      m_experimantal[idx] = QVector<QPointF>(kPointsCount);
      m_approximation[idx] = QVector<QPointF>(kPointsCount);
    }
  }

  /**
   * @brief load_result - метод загружает результат симуляции.
   * @param index - номер точки.
   * @param load - коэффициент нагрузки СМО.
   * @param result - результат симуляции работы СМО.
   */
  void load_result(int index, const qreal load,
                   const queueing_system::SimulationResult& result) noexcept {
    m_experimantal[kWaitFirst][index] = {load, result.avg_wait.first};
    m_experimantal[kWaitSecond][index] = {load, result.avg_wait.second};
    m_experimantal[kUtilityFirst][index] = {load, result.avg_utility.first};
    m_experimantal[kUtilitySecond][index] = {load, result.avg_utility.second};
    m_experimantal[kAVGFirst][index] = {load, result.avg_requests.first};
    m_experimantal[kAVGSecond][index] = {load, result.avg_requests.second};
  }

  /**
   * @brief processResults - расчёты аппроксимации и границ графиков.
   */
  void processResults() {
    for (int graph = 0; graph < kGraphsCount; ++graph) {
      calcYRanges(graph);
      approximate(graph);
    }
  }

  /**
   * @brief pointsExperimental - геттер вектора точек указанного графика.
   * @param graph - номер графика (PointsData::Graph).
   * @return
   */
  const QVector<QPointF>& pointsExperimental(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_experimantal.at(graph);
  }

  /**
   * @brief pointsApproximation - геттер вектора точек аппроксимации указанного
   * графика.
   * @param graph - номер графика (PointsData::Graph).
   * @return
   */
  const QVector<QPointF>& pointsApproximation(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_approximation.at(graph);
  }
  /**
   * @brief RangeX - граница по оси X указанного графика.
   * @param graph - номер графика (PointsData::Graph).
   * @return
   */
  constexpr std::pair<qreal, qreal> RangeX(int graph) const noexcept {
    return {PointsData::kMinLoad, PointsData::kMaxLoad};
  }
  /**
   * @brief RangeY - граница по оси Y указанного графика
   * @param graph - номер графика (PointsData::Graph).
   * @return
   */
  std::pair<qreal, qreal> RangeY(int graph) const noexcept {
    Q_ASSERT(Graph::kWaitFirst <= graph && graph <= Graph::kAVGSecond);
    return m_y_ranges[graph];
  }

private:
  /**
   * @brief calcYRanges - расчёт вертикальных границ графиков.
   * @param graph - номер графика (PointsData::Graph).
   */
  void calcYRanges(int graph) {
    const auto [min, max] = std::minmax_element(
        std::cbegin(m_experimantal[graph]), std::cend(m_experimantal[graph]),
        [](const QPointF& lhs, const QPointF& rhs) {
          return lhs.y() < rhs.y();
        });
    m_y_ranges[graph] = {min->y(), max->y()};
  }

  /**
   * @brief approximate - апроксимация указанного графика с помощью МНК для
   * экспоненциальной зависимости (логорифмирование значений).
   * @param graph - номер графика (PointsData::Graph).
   */
  void approximate(const int graph) {
    constexpr int N = kPointsCount;
    double sx = 0;
    double sx2 = 0;
    double sy = 0;
    double sxy = 0;
    for (auto& point : m_experimantal[graph]) {
      const auto logy = std::log(point.y());
      sx += point.x();
      sx2 += point.x() * point.x();
      sy += logy;
      sxy += point.x() * logy;
    }
    const auto b = (N * sxy - sx * sy) / (N * sx2 - sx * sx);
    const auto a = std::exp((sy - b * sx) / N);
    for (auto index = 0; index < N; ++index) {
      m_approximation[graph][index] =
          QPointF{m_experimantal[graph][index].x(),
                  a * std::exp(b * m_experimantal[graph][index].x())};
    }
  }
};

#endif // POINTS_DATA_H
