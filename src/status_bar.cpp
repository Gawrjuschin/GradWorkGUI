#include "status_bar.h"
#include "progress.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>

constexpr auto kProgressUpdateInterval{100};

StatusBar::StatusBar(const Progress& progress, QWidget* parent)
    : QStatusBar(parent)
    , r_progress(progress)
    , p_progressbar(new QProgressBar(this))
    , p_text(new QLabel(this))
    , p_timer(new QTimer(this))
{
  auto* wdgt = new QWidget();
  auto* lo = new QHBoxLayout(wdgt);
  lo->addWidget(p_progressbar, 0);
  lo->setContentsMargins({0, 0, 0, 0});
  lo->addStretch(1);
  addWidget(wdgt);
  addPermanentWidget(p_text);
  p_text->setText(tr("Waiting for input."));

  connect(p_timer, &QTimer::timeout, this, &StatusBar::onUpdate);
}

StatusBar::~StatusBar() = default;

void StatusBar::setMaximum(int maximum)
{
  p_progressbar->setMaximum(maximum);
}

void StatusBar::setProgressBarWidth(int width)
{
  p_progressbar->setFixedWidth(width);
}

void StatusBar::onStop()
{
  p_progressbar->setValue(0);
  p_text->setText(tr("Waiting for input."));
  p_timer->stop();
}

void StatusBar::onUpdate() {
  const auto progress_displayed = r_progress.value() + 1;
  if (progress_displayed < p_progressbar->maximum()) {
    p_text->setText(tr("(%1/%2) simulations completed.")
                        .arg(progress_displayed)
                        .arg(p_progressbar->maximum()));
    p_progressbar->setValue(progress_displayed);
  } else {
    p_text->setText(tr("Simulation done. Ready for input."));
    p_progressbar->setValue(p_progressbar->maximum());
  }
  p_timer->start(kProgressUpdateInterval);
}
void StatusBar::onStart() {
  p_progressbar->setValue(0);
  p_timer->start();
}
void StatusBar::onReady()
{
  p_timer->stop();
  p_progressbar->setValue(p_progressbar->maximum());
  p_text->setText(tr("(%1/%2) simulations completed.")
                      .arg(p_progressbar->maximum())
                      .arg(p_progressbar->maximum()));
}
