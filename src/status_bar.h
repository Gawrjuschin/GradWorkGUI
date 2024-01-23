#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QStatusBar>
#include <memory>

class Progress;

class QProgressBar;
class QLabel;

class Status_Bar : public QStatusBar
{
  Q_OBJECT

public:
  explicit Status_Bar(const Progress& progress, QWidget* parent = nullptr);
  ~Status_Bar();

  void setMaximum(int maximum);
  void setProgressBarWidth(int width);

public slots:
  void onStop();
  void slot_update();
  void slot_start();
  void onReady();

private:
  const Progress& r_progress;
  QProgressBar* p_progressbar;
  QLabel* p_text;
  QTimer* p_timer;
};

#endif // STATUS_BAR_H
