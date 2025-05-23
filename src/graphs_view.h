#ifndef GRAPHS_VIEW_H
#define GRAPHS_VIEW_H

#include <QChartView>

/**
 * @brief The GraphsView class - виджет отображения чарта с поддержкой
 * увеличения, перетаскивания и сохранения в файл.
 */
class GraphsView : public QChartView {
  Q_OBJECT

  QPoint m_drag_pos{0, 0};

public:
  explicit GraphsView(QWidget* parent = nullptr);
  explicit GraphsView(QChart* chart, QWidget* parent = nullptr);

  ~GraphsView();

public slots:
  /**
   * @brief onZoomIn - слот увеличения, вызывающий
   */
  void onZoomIn() { chart()->zoomIn(); }
  /**
   * @brief onZoomOut
   */
  void onZoomOut() { chart()->zoomOut(); }
  /**
   * @brief onZoomReset
   */
  void onZoomReset() { chart()->zoomReset(); }

  /**
   * @brief onSave - сохранение текущего чарта в файл
   * @param filename - имя файла
   */
  void onSave(QString filename);

protected:
  /**
   * @brief wheelEvent - реализация зума по колёсику мыши
   * @param event
   */
  void wheelEvent(QWheelEvent* event) override;
  /**
   * @brief mousePressEvent - реализация перетаскивания
   * @param event
   */
  void mousePressEvent(QMouseEvent* event) override;
  /**
   * @brief mouseMoveEvent - реализация перетаскивания
   * @param event
   */
  void mouseMoveEvent(QMouseEvent* event) override;
  /**
   * @brief mouseReleaseEvent - реализация перетаскивания
   * @param event
   */
  void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // GRAPHS_VIEW_H
