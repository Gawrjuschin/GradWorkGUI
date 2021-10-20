#include "Widgets/Graphs/graphs_view.h"

#include <QApplication>
#include <QFileDialog>
#include <QObject>

Graphs_View::Graphs_View( QWidget* parent )
  : QChartView( parent )
{
  setDragMode( QGraphicsView::NoDrag );
  this->setMouseTracking( true );
  setRenderHint( QPainter::Antialiasing );
  setContentsMargins( {0,0,0,0} );
  setFrameShape( QFrame::NoFrame );
  setMinimumSize( {640,480} );
}

Graphs_View::Graphs_View(QChart* chart, QWidget* parent)
  : QChartView(chart, parent)
{
  setDragMode(QGraphicsView::NoDrag);
  this->setMouseTracking(true);
  setRenderHint(QPainter::Antialiasing);
  setContentsMargins( {0,0,0,0} );
  setFrameShape(QFrame::NoFrame);
  setMinimumSize( {640,480} );
}

Graphs_View::~Graphs_View( ) = default;

void Graphs_View::slot_zoom_in()
{
  chart()->zoomIn();
}

void Graphs_View::slot_zoom_out()
{
  chart()->zoomOut();
}

void Graphs_View::slot_zoom_reset()
{
  chart()->zoomReset();
}

void Graphs_View::slot_save(QString filename)
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

void Graphs_View::wheelEvent(QWheelEvent *event)
{
  if(event->angleDelta().x() < 0 || event->angleDelta().y() > 0)
    {
      chart()->zoomIn();
    }
  else
    {
      chart()->zoomOut();
    }
}

void Graphs_View::mousePressEvent(QMouseEvent* event)
{
  if( rect().contains(event->pos()) && event->button() == Qt::LeftButton )
    {
      setCursor(QCursor(Qt::OpenHandCursor));
      m_drag_pos = event->pos();
      event->accept();
    }
  QChartView::mousePressEvent(event);
}

void Graphs_View::mouseMoveEvent(QMouseEvent* event)
{
  if(event->buttons() == Qt::LeftButton)
    {
      setCursor(QCursor(Qt::ClosedHandCursor));
      auto dpos = event->pos() - m_drag_pos;
      chart()->scroll(-dpos.x(),dpos.y());
      m_drag_pos = event->pos();
      event->accept();
    }
  QChartView::mouseMoveEvent(event);
}

void Graphs_View::mouseReleaseEvent(QMouseEvent* event)
{
  if(event->button() == Qt::LeftButton)
    {
      setCursor(QCursor(Qt::ArrowCursor));
      event->accept();
    }
  QChartView::mouseMoveEvent(event);
}
