#ifndef LOADING_WIDGET_H
#define LOADING_WIDGET_H

#include <QWidget>

class QLabel;

class Loading_Widget : public QWidget
{
  Q_OBJECT

  Q_PROPERTY(QPixmap pixmap READ pixmap WRITE onSetPixmap NOTIFY pixmapChanged)

public:
  explicit Loading_Widget(const int pix_size, QWidget* parent = nullptr);
  ~Loading_Widget();

  QPixmap pixmap() const;

public slots:
  void onStop();
  void onStart();
  void onSetPixmap(const QPixmap& pic);

protected slots:
  void onNextText();
  void onNextPic();

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
