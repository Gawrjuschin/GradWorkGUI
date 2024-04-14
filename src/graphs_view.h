#ifndef GRAPHS_VIEW_H
#define GRAPHS_VIEW_H

#include <QtCharts>

class Graphs_View : public QChartView
{
  Q_OBJECT

public:
  explicit Graphs_View(QWidget* parent = nullptr);
  explicit Graphs_View(QChart* chart, QWidget* parent = nullptr);
  ~Graphs_View();

public slots:
  void onZoomIn();
  void onZoomOut();
  void onZoomReset();
  void onSave(QString);

protected:
  void wheelEvent(QWheelEvent*) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

private:
  QPoint m_drag_pos{0, 0};
};

#endif // GRAPHS_VIEW_H
