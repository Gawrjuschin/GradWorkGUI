#ifndef GRAPHS_VIEW_H
#define GRAPHS_VIEW_H

#include <QtCharts>

class Graphs_View : public QChartView
{
  Q_OBJECT

public:
  Graphs_View(QWidget* parent = nullptr);
  Graphs_View(QChart* chart, QWidget* parent = nullptr);
  ~Graphs_View();

public slots:
  void slot_zoom_in();
  void slot_zoom_out();
  void slot_zoom_reset();
  void slot_save(QString);

protected:
  virtual void wheelEvent(QWheelEvent*) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
  QPoint m_drag_pos{0, 0};
};

#endif // GRAPHS_VIEW_H
