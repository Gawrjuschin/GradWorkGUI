#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QStatusBar>

class Progress;

class QProgressBar;
class QLabel;

/**
 * @brief The StatusBar class - виджет статусбара с индикацией прогресса
 */
class StatusBar : public QStatusBar {
  Q_OBJECT

  const Progress& r_progress;
  QProgressBar* p_progressbar;
  QLabel* p_text;
  QTimer* p_timer;

public:
  explicit StatusBar(const Progress& progress, QWidget* parent = nullptr);

  ~StatusBar();

  /**
   * @brief setMaximum - установить максимальное значение прогресса
   * @param maximum - новый максимум
   */
  void setMaximum(int maximum);
  /**
   * @brief setProgressBarWidth - установить ширину виджетп
   * @param width - ширина
   */
  void setProgressBarWidth(int width);

public slots:
  /**
   * @brief onStop - при остановке симуляции сбрасывает прогресс и останавливает
   * таймер
   */
  void onStop();
  /**
   * @brief onUpdate - слот таймера обновления прогресса
   */
  void onUpdate();
  /**
   * @brief onStart - запустить таймер обновления прогресса
   */
  void onStart();
  /**
   * @brief onReady - при готовности останавливает таймер и меняет сообщение
   */
  void onReady();
};

#endif // STATUS_BAR_H
