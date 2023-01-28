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
  explicit Status_Bar( std::shared_ptr<Progress> progress, QWidget* parent = nullptr );
  ~Status_Bar( );

  void setMaximum(int maximum);
  void setProgressBarWidth(int width);

public slots:
  void slot_reset();
  void slot_update();
  void slot_start();
  void slot_stop();

private:
  QProgressBar*  p_progressbar;
  QLabel*        p_text;
  std::shared_ptr<Progress>      p_progress;
  QTimer*        p_timer;
};

#endif // STATUS_BAR_H
