#include "input_widget.h"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QStyleOption>
#include <QThread>
// #include <QtMath>

#include <cstdint>
#include <limits>

constexpr double kSingleStep = 0.01;
constexpr int kMaxChannels = 16;

constexpr int kMinEvents = 1'000;
constexpr int kMaxEvents = 1'000'000;
static_assert(kMinEvents >= 0);
static_assert(kMaxEvents <= std::numeric_limits<std::uint32_t>::max());

QWidget* InputWidget::makeSystemGrpbx() {
  auto* system_grbx = new QGroupBox(tr("System parameters"));
  system_grbx->setAlignment(Qt::AlignCenter);

  auto* system_lo = new QGridLayout(system_grbx);

  auto* load_lbl = new QLabel(tr("Load range:"));
  auto* load_desc =
      new QLabel(QString(tr("[%1;  %2]")).arg(m_min_load).arg(m_max_load));
  system_lo->addWidget(load_lbl, 0, 0);
  system_lo->addWidget(load_desc, 0, 1);

  auto* lambda_lbl = new QLabel(tr("Lambda range:"));
  p_lambda_range = new QLabel;
  system_lo->addWidget(lambda_lbl, 1, 0);
  system_lo->addWidget(p_lambda_range, 1, 1);

  constexpr double kDefaultMu = 1.0;
  auto* mu_lbl = new QLabel(tr("&mu parameter:"));
  p_mu_input = new QDoubleSpinBox;
  p_mu_input->setValue(kDefaultMu);
  p_mu_input->setMinimum(kSingleStep);
  p_mu_input->setSingleStep(kSingleStep);
  mu_lbl->setBuddy(p_mu_input);
  system_lo->addWidget(mu_lbl, 2, 0);
  system_lo->addWidget(p_mu_input, 2, 1);

  constexpr int kDefaultChannels = 1;
  auto* ch_lbl = new QLabel(tr("Number of &channels:"));
  p_ch_input = new QSpinBox;
  p_ch_input->setValue(kDefaultChannels);
  p_ch_input->setRange(kDefaultChannels, kMaxChannels);
  ch_lbl->setBuddy(p_ch_input);
  system_lo->addWidget(ch_lbl, 3, 0);
  system_lo->addWidget(p_ch_input, 3, 1);

  constexpr double kDefaultProp = 0.25;
  constexpr double kMinProp = 0.00;
  constexpr double kMaxProp = 1.00;
  auto* pr_lbl = new QLabel(tr("First priority &propability:"));
  p_pr_input = new QDoubleSpinBox;
  p_pr_input->setValue(kDefaultProp);
  p_pr_input->setRange(kMinProp, kMaxProp);
  p_pr_input->setSingleStep(kSingleStep);
  pr_lbl->setBuddy(p_pr_input);
  system_lo->addWidget(pr_lbl, 4, 0);
  system_lo->addWidget(p_pr_input, 4, 1);

  onUpdate();

  connect(p_mu_input,
          static_cast<void (QDoubleSpinBox::*)(double)>(
              &QDoubleSpinBox::valueChanged),
          this, &InputWidget::onUpdate);
  connect(p_ch_input,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
          &InputWidget::onUpdate);

  return system_grbx;
}

QWidget* InputWidget::makeSimulationGrpbx() {
  auto* simulation_grbx = new QGroupBox(tr("Simulation parameters"));
  simulation_grbx->setAlignment(Qt::AlignCenter);

  auto* simulation_lo = new QGridLayout(simulation_grbx);

  auto* threads_lbl = new QLabel(tr("Number of &threads"));
  p_threads_input = new QSpinBox;
  p_threads_input->setRange(1, QThread::idealThreadCount());
  threads_lbl->setBuddy(p_threads_input);
  simulation_lo->addWidget(threads_lbl, 0, 0);
  simulation_lo->addWidget(p_threads_input, 0, 1);
  auto* events_lbl = new QLabel(tr("Number of &events"));
  p_events_input = new QSpinBox;
  p_events_input->setRange(kMinEvents, kMaxEvents);
  events_lbl->setBuddy(p_events_input);
  simulation_lo->addWidget(events_lbl, 1, 0);
  simulation_lo->addWidget(p_events_input, 1, 1);

  return simulation_grbx;
}

QWidget* InputWidget::makeControlsGrpbx() {
  auto* control_grbx = new QGroupBox(tr("Process control"));
  control_grbx->setAlignment(Qt::AlignCenter);

  auto* control_lo = new QGridLayout(control_grbx);

  p_stop_btn = new QPushButton(tr("S&top"));
  p_stop_btn->setDisabled(true);
  control_lo->addWidget(p_stop_btn, 0, 0);

  p_pause_btn = new QPushButton(tr("&Pause"));
  p_pause_btn->setDisabled(true);
  control_lo->addWidget(p_pause_btn, 0, 1);

  p_start_btn = new QPushButton(tr("&Start"));
  control_lo->addWidget(p_start_btn, 0, 2);

  connect(p_start_btn, &QPushButton::clicked, this, &InputWidget::onStart);
  connect(p_pause_btn, &QPushButton::clicked, this,
          &InputWidget::onPauseResume);
  connect(p_stop_btn, &QPushButton::clicked, this, &InputWidget::onStop);

  return control_grbx;
}

InputWidget::InputWidget(double min_load, double max_load, QWidget* parent)
    : QWidget(parent), m_min_load{min_load}, m_max_load{max_load} {
  auto* widget_lo = new QVBoxLayout(this);

  widget_lo->addStretch(1);

  widget_lo->addWidget(makeSystemGrpbx(), 0);
  widget_lo->addWidget(makeSimulationGrpbx(), 0);
  widget_lo->addWidget(makeControlsGrpbx(), 0);

  widget_lo->addStretch(1);
}

InputWidget::~InputWidget() = default;

const InputData& InputWidget::inputData() const noexcept { return m_data; }

void InputWidget::onUpdate() {
  auto lambdaLower = QString().setNum(
      p_mu_input->value() * p_ch_input->value() * m_min_load, 'g', 4);
  auto lambdaUpper = QString().setNum(
      p_mu_input->value() * p_ch_input->value() * m_max_load, 'g', 4);

  p_lambda_range->setText(
      QString(tr("[%1;  %2]")).arg(lambdaLower).arg(lambdaUpper));
}

void InputWidget::loadData() {
  m_data =
      InputData{.mu = p_mu_input->value(),
                .propability = p_pr_input->value(),
                .channels = p_ch_input->value(),
                .threads = p_threads_input->value(),
                .events = static_cast<std::uint32_t>(p_events_input->value())};
}

void InputWidget::onDone() {
  p_start_btn->setDisabled(false);
  p_pause_btn->setDisabled(true);
  p_stop_btn->setDisabled(true);
}

void InputWidget::onStart() {
  p_start_btn->setDisabled(true);
  p_pause_btn->setDisabled(false);
  p_stop_btn->setDisabled(false);
  loadData();
  emit sigStart();
}

void InputWidget::onPauseResume() {
  if ((m_paused = (!m_paused))) {
    p_pause_btn->setText(tr("Resume"));
    emit sigPause();
  } else {
    p_pause_btn->setText(tr("Pause"));
    emit sigResume();
  }
}

void InputWidget::onStop() {
  p_start_btn->setDisabled(false);
  p_pause_btn->setDisabled(true);
  p_stop_btn->setDisabled(true);
  p_pause_btn->setText(tr("Pause"));
  m_paused = false;
  emit sigStop();
}
