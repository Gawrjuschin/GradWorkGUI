#include "status_bar.h"
#include "progress.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>

constexpr auto timer_interval = 100;

Status_Bar::Status_Bar( std::shared_ptr<Progress> progress, QWidget* parent )
  : QStatusBar(parent)
  , p_progressbar( new QProgressBar(this) )
  , p_text( new QLabel(this) )
  , p_progress( progress )
  , p_timer(new QTimer(this))
{
  auto* wdgt = new QWidget();
  auto* lo = new QHBoxLayout(wdgt);
  lo->addWidget(p_progressbar, 0);
  lo->setContentsMargins({0,0,0,0});
  lo->addStretch(1);
  addWidget(wdgt);
  addPermanentWidget(p_text);
  p_text->setText(tr("Waiting for input."));

  connect(p_timer, &QTimer::timeout, this, &Status_Bar::slot_update);
}

Status_Bar::~Status_Bar() = default;

void Status_Bar::setMaximum(int maximum)
{
  p_progressbar->setMaximum(maximum);
}

void Status_Bar::setProgressBarWidth(int width)
{
  p_progressbar->setFixedWidth(width);
}

void Status_Bar::slot_reset()
{
  p_progress->resetProgress( );
  p_progressbar->setValue( 0 );
  p_text->setText(tr("Waiting for input."));
  p_timer->stop();
}

void Status_Bar::slot_update()
{
  auto temp = p_progress->getProgress();
  if(temp < p_progressbar->maximum())
    {
      p_text->setText(tr("(%1/%2) simulations completed.")
                      .arg(temp)
                      .arg(p_progressbar->maximum()));
      p_progressbar->setValue(temp);
    }
  else
    {
      p_text->setText(tr("Simulation done. Ready for input."));
      p_progressbar->setValue(p_progressbar->maximum());
    }
  p_timer->start(timer_interval);
}
void Status_Bar::slot_start()
{
  p_progress->resetProgress( );
  p_progressbar->setValue( 0 );
  p_timer->start();
}
void Status_Bar::slot_stop()
{
  p_timer->stop();
}
