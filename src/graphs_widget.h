#ifndef GRAPHS_WIDGET_H
#define GRAPHS_WIDGET_H

#include <QWidget>
#include <QtCharts>
#include <array>
#include <memory>

class Graphs_Switch;
class Graphs_View;
class PointsData;

class Graphs_Widget : public QWidget
{
  Q_OBJECT

  enum { GRAPHS_COUNT = 6 };

public:
  explicit Graphs_Widget(const PointsData& points_data,
                         QWidget* parent = nullptr);
  ~Graphs_Widget();

protected:
  void paintEvent(QPaintEvent* event) override;

public slots:
  void onPointsReady();
  void onStop();

protected slots:
  void onShow(int index);
  void onApproximate(int index);
  void onDeapproximate(int index);

signals:
  void sigEnd();

private:
  const PointsData& r_points_data;
  Graphs_Switch* p_chart_switch;
  Graphs_View* p_chart_view;
  std::array<std::unique_ptr<QChart>, GRAPHS_COUNT> charts_array{};
  std::array<QLineSeries*, GRAPHS_COUNT> exp_series_array{};
  std::array<QLineSeries*, GRAPHS_COUNT> apr_series_array{};

private:
  void adjust_charts(int index);
  void update_series(int index);
};

#endif // GRAPHS_WIDGET_H
