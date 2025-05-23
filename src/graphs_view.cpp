#include "graphs_view.h"

#include <QApplication>
#include <QFileDialog>
#include <QObject>

GraphsView::GraphsView(QWidget* parent)
    : QChartView(parent)
{
  setDragMode(QGraphicsView::NoDrag);
  this->setMouseTracking(true);
  setRenderHint(QPainter::Antialiasing);
  setContentsMargins({0, 0, 0, 0});
  setFrameShape(QFrame::NoFrame);
  setMinimumSize({640, 480});
}

GraphsView::GraphsView(QChart* chart, QWidget* parent)
    : QChartView(chart, parent)
{
  setDragMode(QGraphicsView::NoDrag);
  this->setMouseTracking(true);
  setRenderHint(QPainter::Antialiasing);
  setContentsMargins({0, 0, 0, 0});
  setFrameShape(QFrame::NoFrame);
  setMinimumSize({640, 480});
}

GraphsView::~GraphsView() = default;

void GraphsView::onSave(QString filename)
{
  QPixmap pixmap(this->size());
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  render(&painter);
  auto savepath = QFileDialog::getSaveFileName(this,
					       tr("Save graph"),
					       filename,
					       QString::fromLatin1("PNG (*.png);;JPEG (*.jpg)"));
  pixmap.save(savepath);
}

void GraphsView::wheelEvent(QWheelEvent* event)
{
  if (event->angleDelta().x() < 0 || event->angleDelta().y() > 0) {
    chart()->zoomIn();
  } else {
    chart()->zoomOut();
  }
}

void GraphsView::mousePressEvent(QMouseEvent* event)
{
  if (rect().contains(event->pos()) && event->button() == Qt::LeftButton) {
    setCursor(QCursor(Qt::OpenHandCursor));
    m_drag_pos = event->pos();
    event->accept();
  }
  QChartView::mousePressEvent(event);
}

void GraphsView::mouseMoveEvent(QMouseEvent* event)
{
  if (event->buttons() == Qt::LeftButton) {
    setCursor(QCursor(Qt::ClosedHandCursor));
    auto dpos = event->pos() - m_drag_pos;
    chart()->scroll(-dpos.x(), dpos.y());
    m_drag_pos = event->pos();
    event->accept();
  }
  QChartView::mouseMoveEvent(event);
}

void GraphsView::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    setCursor(QCursor(Qt::ArrowCursor));
    event->accept();
  }
  QChartView::mouseMoveEvent(event);
}
