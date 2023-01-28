#ifndef GRAPHS_WIDGET_H
#define GRAPHS_WIDGET_H

#include <QWidget>
#include <QtCharts>
#include <array>
#include <memory>

class Graphs_Switch;
class Graphs_View;
class Graphs_Data;

class Graphs_Widget : public QWidget
{
  Q_OBJECT

  enum
  {
    GRAPHS_COUNT = 6
  };

public:
  explicit Graphs_Widget(Graphs_Data* gdata, QWidget *parent = nullptr);
  ~Graphs_Widget( );

protected:
  virtual void paintEvent(QPaintEvent *event) override;

public slots:
  void slot_end();
  void slot_stop();

protected slots:
  void slot_show(int index);
  void slot_approximate(int index);
  void slot_deapproximate(int index);

signals:
  void signal_end();

private:
  Graphs_Switch*    p_chart_switch;
  Graphs_View*      p_chart_view;
  std::array<std::unique_ptr<QChart>, GRAPHS_COUNT>  p_vector_charts;
  Graphs_Data*      p_points_data;

private:
  void adjust_graphs(int index);
  void update_series(int index);

};

#endif // GRAPHS_WIDGET_H
