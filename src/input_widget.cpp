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
#include <QtMath>

constexpr double single_step = 0.01;
constexpr int min_evnum = 1'000;
constexpr int max_evnum = 1'000'000;
constexpr int max_channels = 16;

Input_Widget::Input_Widget(QWidget* parent)
    : QWidget(parent)
    , p_parameter_desc(new QLabel(tr("empty"), this))
    , p_requests_desc(new QLabel(tr("empty"), this))
    , p_mu_input(new QDoubleSpinBox(this))
    , p_ch_input(new QSpinBox(this))
    , p_pr_input(new QDoubleSpinBox(this))
    , p_th_input(new QSpinBox(this))
    , p_ev_input(new QSpinBox(this))
    , p_stop_btn(new QPushButton(tr("S&top"), this))
    , p_pause_btn(new QPushButton(tr("&Pause"), this))
    , p_start_btn(new QPushButton(tr("&Start"), this))
{
  auto* widget_lo = new QVBoxLayout(this);

  auto* system_grbx = new QGroupBox(tr("System parameters"), this);
  auto* system_lo = new QGridLayout(system_grbx);

  auto* simulation_grbx = new QGroupBox(tr("Simulation parameters"), this);
  auto* simulation_lo = new QGridLayout(simulation_grbx);

  auto* control_grbx = new QGroupBox(tr("Process control"), this);
  auto* control_lo = new QGridLayout(control_grbx);

  //build_first();
  // Размещение групп виджетов
  {
    widget_lo->addStretch(1);
    system_grbx->setAlignment(Qt::AlignCenter);
    widget_lo->addWidget(system_grbx, 0);
    simulation_grbx->setAlignment(Qt::AlignCenter);
    widget_lo->addWidget(simulation_grbx, 0);
    control_grbx->setAlignment(Qt::AlignCenter);
    widget_lo->addWidget(control_grbx, 0);
    widget_lo->addStretch(1);
  }

  // build_second();
  // Настройка виджетов параметров системы
  {
    auto* load_lbl = new QLabel(tr("Load range:"), this);
    auto* load_desc
	= new QLabel(QString(tr("[%1;  %2]")).arg(InputData::min_load).arg(InputData::max_load),
		     this);
    auto* par_lbl = new QLabel(tr("Lambda range:"), this);

    auto* requests_lbl = new QLabel(tr("AVG requests:"), this);

    auto* mu_lbl = new QLabel(tr("&mu parameter:"), this);
    mu_lbl->setBuddy(p_mu_input);
    auto* ch_lbl = new QLabel(tr("Number of &channels:"), this);
    ch_lbl->setBuddy(p_ch_input);
    auto* pr_lbl = new QLabel(tr("First priority &propability:"), this);
    pr_lbl->setBuddy(p_pr_input);

    p_mu_input->setValue(1);
    p_mu_input->setMinimum(single_step);
    p_mu_input->setSingleStep(single_step);
    p_ch_input->setValue(1);
    p_ch_input->setRange(1, max_channels);
    p_pr_input->setValue(0.25);
    p_pr_input->setRange(single_step, 1.00);
    p_pr_input->setSingleStep(single_step);
    p_parameter_desc->setText(
	QString(tr("[%1;  %2]"))
	    .arg(QString().setNum(p_mu_input->value() * p_ch_input->value() * InputData::min_load,
				  'g',
				  4))
	    .arg(QString().setNum(p_mu_input->value() * p_ch_input->value() * InputData::max_load,
				  'g',
				  4)));
    p_requests_desc->setText(
	QString(tr("[%1;  %2]"))
	    .arg(QString().setNum(
		1 / (p_mu_input->value() * p_ch_input->value() * InputData::max_load), 'g', 4))
	    .arg(QString().setNum(
		1 / (p_mu_input->value() * p_ch_input->value() * InputData::min_load), 'g', 4)));

    system_lo->addWidget(load_lbl, 0, 0);
    system_lo->addWidget(load_desc, 0, 1);
    system_lo->addWidget(par_lbl, 1, 0);
    system_lo->addWidget(p_parameter_desc, 1, 1);
    system_lo->addWidget(requests_lbl, 2, 0);
    system_lo->addWidget(p_requests_desc, 2, 1);
    system_lo->addWidget(mu_lbl, 3, 0);
    system_lo->addWidget(p_mu_input, 3, 1);
    system_lo->addWidget(ch_lbl, 4, 0);
    system_lo->addWidget(p_ch_input, 4, 1);
    system_lo->addWidget(pr_lbl, 5, 0);
    system_lo->addWidget(p_pr_input, 5, 1);
  }

  // build_third();
  {
    auto* th_lbl = new QLabel(tr("Number of &threads"), this);
    th_lbl->setBuddy(p_th_input);
    auto* ev_lbl = new QLabel(tr("Number of &events"), this);
    ev_lbl->setBuddy(p_ev_input);
    p_th_input->setRange(1, QThread::idealThreadCount());
    p_ev_input->setRange(min_evnum, max_evnum);
    simulation_lo->addWidget(th_lbl, 0, 0);
    simulation_lo->addWidget(p_th_input, 0, 1);
    simulation_lo->addWidget(ev_lbl, 1, 0);
    simulation_lo->addWidget(p_ev_input, 1, 1);
  }

  // build_fourth();
  {
    p_stop_btn->setDisabled(true);
    p_pause_btn->setDisabled(true);
    control_lo->addWidget(p_stop_btn, 0, 0);
    control_lo->addWidget(p_pause_btn, 0, 1);
    control_lo->addWidget(p_start_btn, 0, 2);
  }

  // Настройки кнопок управления
  {
    connect(p_start_btn, &QPushButton::clicked, this, &Input_Widget::slot_start);
    connect(p_pause_btn, &QPushButton::clicked, this, &Input_Widget::slot_pause_resume);
    connect(p_stop_btn, &QPushButton::clicked, this, &Input_Widget::slot_stop);
  }
  // Настройки спинбоксов
  {
    connect(p_mu_input,
	    static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
	    this,
	    &Input_Widget::update);
    connect(p_ch_input,
	    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	    this,
	    &Input_Widget::update);
  }
}

Input_Widget::~Input_Widget() = default;

const InputData& Input_Widget::data() const noexcept
{
  return m_data;
}

void Input_Widget::paintEvent(QPaintEvent* event)
{
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void Input_Widget::update()
{
  p_parameter_desc->setText(
      QString(tr("[%1;  %2]"))
	  .arg(QString().setNum(p_mu_input->value() * p_ch_input->value() * InputData::min_load,
				'g',
				4))
	  .arg(QString().setNum(p_mu_input->value() * p_ch_input->value() * InputData::max_load,
				'g',
				4)));
  p_requests_desc->setText(
      QString(tr("[%1;  %2]"))
	  .arg(
	      QString().setNum(1 / (p_mu_input->value() * p_ch_input->value() * InputData::max_load),
			       'g',
			       4))
	  .arg(
	      QString().setNum(1 / (p_mu_input->value() * p_ch_input->value() * InputData::min_load),
			       'g',
			       4)));
}

void Input_Widget::load_data()
{
  m_data = InputData{.mu = p_mu_input->value(),
		     .propability = p_pr_input->value(),
		     .channels = p_ch_input->value(),
		     .threads = p_th_input->value(),
		     .events = p_ev_input->value()};
}

void Input_Widget::slot_done()
{
  p_start_btn->setDisabled(false);
  p_pause_btn->setDisabled(true);
  p_stop_btn->setDisabled(true);
}

void Input_Widget::slot_start()
{
  p_start_btn->setDisabled(true);
  p_pause_btn->setDisabled(false);
  p_stop_btn->setDisabled(false);
  load_data();
  emit signal_start();
}

void Input_Widget::slot_pause_resume()
{
  if ((m_paused = (!m_paused))) {
    p_pause_btn->setText(tr("Resume"));
    emit signal_pause();
  } else {
    p_pause_btn->setText(tr("Pause"));
    emit signal_resume();
  }
}

void Input_Widget::slot_stop()
{
  p_start_btn->setDisabled(false);
  p_pause_btn->setDisabled(true);
  p_stop_btn->setDisabled(true);
  p_pause_btn->setText(tr("Pause"));
  m_paused = false;
  emit sigStop();
}
