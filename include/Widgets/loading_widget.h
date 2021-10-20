#ifndef LOADING_WIDGET_H
#define LOADING_WIDGET_H

#include <QWidget>

class QLabel;

class Loading_Widget : public QWidget
{
  Q_OBJECT

  Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged)

public:
  explicit Loading_Widget(const int pix_size, QWidget *parent = nullptr);
  ~Loading_Widget();

  QPixmap pixmap() const;

public slots:
  void slot_stop();
  void slot_start();
  void setPixmap(const QPixmap& pic);

protected slots:
  void next_text();
  void next_pic();

signals:
  void pixmapChanged();

private:
  QLabel* p_text;
  QLabel* p_pic_lbl;
  QGraphicsProxyWidget* p_proxy;
  int m_pix_size;

  QTimer* p_timer_text;
  QTimer* p_timer_pic;
  int m_stage{1};

private:
  void rotate_pic(double angle);
};

#endif // LOADING_WIDGET_H
