#ifndef LOADING_WIDGET_H
#define LOADING_WIDGET_H

#include <QWidget>

class QLabel;

class LoadingWidget : public QWidget {
  Q_OBJECT

  Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged)

public:
  explicit LoadingWidget(const int pix_size, QWidget* parent = nullptr);
  ~LoadingWidget();

  QPixmap pixmap() const;
  void setPixmap(const QPixmap& pic);

public slots:
  void onStop();
  void onStart();

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
