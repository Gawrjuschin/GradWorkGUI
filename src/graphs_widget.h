#ifndef GRAPHS_WIDGET_H
#define GRAPHS_WIDGET_H

#include "points_data.h"

#include <QChart>
#include <QLineSeries>
#include <QWidget>

#include <array>

class GraphsSwitch;
class GraphsView;

/**
 * @brief The GraphsWidget class - виджет, отвечающий за отображение и
 * переключение графиков. Хранит
 */
class GraphsWidget : public QWidget {
  Q_OBJECT

  const PointsData& r_points_data;
  GraphsSwitch* p_chart_switch;
  GraphsView* p_chart_view;
  std::array<QChart*, PointsData::kGraphsCount> charts_array{};

public:
  explicit GraphsWidget(const PointsData& points_data,
                        QWidget* parent = nullptr);

  ~GraphsWidget();

protected:
  /**
   * @brief paintEvent - перегрузка метода для работы со стилями
   * @param event
   */
  // void paintEvent(QPaintEvent* event) override;

public slots:
  void onPointsReady();

protected slots:
  /**
   * @brief onShow - переключиться на указанный график
   * @param index - индекс графика
   */
  void onShow(int index);
  /**
   * @brief onApproximate - включить отображение апроксимации для указанного
   * графика
   * @param index - индекс графика
   */
  void onApproximate(int index);
  /**
   * @brief onDeapproximate - отключить отображжение апроксимации для указанного
   * графика
   * @param index - индекс графика
   */
  void onDeapproximate(int index);

signals:
  /**
   * @brief sigEnd - сигнал о готовности графиков к отображению
   */
  void sigEnd();

private:
  /**
   * @brief updateSeries - загрузить новые точки в графики и выполнить расчёты
   * отображения по ним
   * @param index - индекс графика
   */
  void updateSeries(int index);

  /**
   * @brief expSeries - извлечение серий экспериментальных точек для указанного
   * графика
   * @param index - индекс графика
   * @return
   */
  QLineSeries* expSeries(int index) const {
    return static_cast<QLineSeries*>(charts_array[index]->series()[0]);
  }
  /**
   * @brief aprSeries - извлечение серий апроксимации точек для указанного
   * графика
   * @param index - индекс графика
   * @return
   */
  QLineSeries* aprSeries(int index) const {
    return static_cast<QLineSeries*>(charts_array[index]->series()[1]);
  }
};

#endif // GRAPHS_WIDGET_H
