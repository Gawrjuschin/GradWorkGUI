#ifndef LOADING_WIDGET_H
#define LOADING_WIDGET_H

#include <QWidget>

class QLabel;

/**
 * @brief The LoadingWidget class - виджет индикатора загрузки. Отображает
 * вращающуюся картинку (throbber) и текст с меняющимся многоточием.
 */
class LoadingWidget : public QWidget {
  Q_OBJECT

  Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged)

  QLabel* p_text;
  QLabel* p_pic_lbl;
  QGraphicsProxyWidget* p_proxy;
  int m_pix_size;

  QTimer* p_timer_text;
  QTimer* p_timer_pic;
  int m_stage{1};

public:
  explicit LoadingWidget(const int pix_size, QWidget* parent = nullptr);

  ~LoadingWidget();

  /**
   * @brief pixmap - изобращение (throbber)
   * @return
   */
  QPixmap pixmap() const;
  /**
   * @brief setPixmap - сеттер изображения (throbber)
   * @param pic
   */
  void setPixmap(const QPixmap& pic);

public slots:
  /**
   * @brief onStop - прекращает анимацию
   */
  void onStop();
  /**
   * @brief onStart - запускает анимацию
   */
  void onStart();

protected slots:
  /**
   * @brief onNextText - переключение многоточия
   */
  void onNextText();
  /**
   * @brief onNextPic - вращение картинки (throbber) и запуск таймера
   */
  void onNextPic();

signals:
  /**
   * @brief pixmapChanged - сигнал замены картинки
   */
  void pixmapChanged();

private:
  /**
   * @brief rotatePic - реализация вращения картинки
   * @param angle
   */
  void rotatePic(double angle);
};

#endif // LOADING_WIDGET_H
