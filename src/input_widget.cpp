#include "input_widget.h"
#include "system_data.h"

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

constexpr double min_load = 0.35;
constexpr double max_load = 0.95;
constexpr double single_step = 0.01;
constexpr int    min_evnum   = 1'000;
constexpr int    max_evnum   = 1'000'000;
constexpr int    max_channels = 16;


Input_Widget::Input_Widget(std::shared_ptr<System_Data> _p_system_parameters, QWidget *parent)
  : QWidget(parent)
  , p_sys_grbx(new QGroupBox(tr("System parameters"), this))
  , p_sim_grbx(new QGroupBox(tr("Simulation parameters"), this))
  , p_ctrl_grbx(new QGroupBox(tr("Process control"), this))
  , p_par_desc(new QLabel(tr("empty"), this))//Переводит среднее время в среднее число
  , p_avg_desc(new QLabel(tr("empty"), this))
  , p_mu_input(new QDoubleSpinBox(this))
  , p_ch_input(new QSpinBox(this))
  , p_pr_input(new QDoubleSpinBox(this))
  , p_th_input(new QSpinBox(this))
  , p_ev_input(new QSpinBox(this))
  , p_stop_btn(new QPushButton(tr("S&top"), this))
  , p_pause_btn(new QPushButton(tr("&Pause"), this))
  , p_start_btn(new QPushButton(tr("&Start"), this))
  , p_system_parameters(_p_system_parameters)
{
  build_first();
  build_second();
  build_third();
  build_fourth();

  p_par_desc->setText(QString(tr("[%1;%2]"))
                      .arg(p_mu_input->value() * p_ch_input->value() * min_load)
                      .arg(p_mu_input->value() * p_ch_input->value() * max_load));
  p_avg_desc->setText(QString(tr("[%1;%2]"))
                      .arg(1/(p_mu_input->value() * p_ch_input->value() * max_load))
                      .arg(1/(p_mu_input->value() * p_ch_input->value() * min_load)));

  connect(p_start_btn, &QPushButton::clicked,
          this,      &Input_Widget::slot_start);
  connect(p_pause_btn,&QPushButton::clicked,
          this,      &Input_Widget::slot_pause_resume);
  connect(p_stop_btn, &QPushButton::clicked,
          this,      &Input_Widget::slot_stop);

  connect(p_mu_input, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this, &Input_Widget::update);
  connect(p_ch_input, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this, &Input_Widget::update);
}

Input_Widget::~Input_Widget() = default;

void Input_Widget::build_first()
{ // Adjust layout
  auto* main_lo = new QVBoxLayout(this);
  main_lo->addStretch(1);
  p_sys_grbx->setAlignment(Qt::AlignCenter);
  main_lo->addWidget(p_sys_grbx,0);
  p_sim_grbx->setAlignment(Qt::AlignCenter);
  main_lo->addWidget(p_sim_grbx,0);
  p_ctrl_grbx->setAlignment(Qt::AlignCenter);
  main_lo->addWidget(p_ctrl_grbx,0);
  main_lo->addStretch(1);
}

void Input_Widget::build_second()
{ // Adjust system elements
  auto*  sys_lo = new QGridLayout(p_sys_grbx);
  auto*  load_lbl = new QLabel(tr("Load range:"), this);
  auto*  load_desc = new QLabel(QString(tr("[%1;\t%2]"))
                              .arg(min_load)
                              .arg(max_load), this);
  auto*  par_lbl = new QLabel(tr("Lambda range:"), this);
  auto*  avg_lbl = new QLabel(tr("Average lambda range:"), this);
  auto*  mu_lbl = new QLabel(tr("&mu parameter:"), this);
  mu_lbl->setBuddy(p_mu_input);
  auto*  ch_lbl = new QLabel(tr("Number of &channels:"), this);
  ch_lbl->setBuddy(p_ch_input);
  auto*  pr_lbl = new QLabel(tr("First priority &propability:"), this);
  pr_lbl->setBuddy(p_pr_input);

  p_mu_input->setValue(1);
  p_mu_input->setMinimum(single_step);
  p_mu_input->setSingleStep(single_step);
  p_ch_input->setValue(1);
  p_ch_input->setRange(1,max_channels);
  p_pr_input->setValue(0.25);
  p_pr_input->setRange(single_step, 1.00);
  p_pr_input->setSingleStep(single_step);

  sys_lo->addWidget(load_lbl,   0, 0);
  sys_lo->addWidget(load_desc,  0, 1);
  sys_lo->addWidget(par_lbl,    1, 0);
  sys_lo->addWidget(p_par_desc, 1, 1);
  sys_lo->addWidget(avg_lbl,    2, 0);
  sys_lo->addWidget(p_avg_desc, 2, 1);
  sys_lo->addWidget(mu_lbl,     3, 0);
  sys_lo->addWidget(p_mu_input, 3, 1);
  sys_lo->addWidget(ch_lbl,     4, 0);
  sys_lo->addWidget(p_ch_input, 4, 1);
  sys_lo->addWidget(pr_lbl,     5, 0);
  sys_lo->addWidget(p_pr_input, 5, 1);
}

void Input_Widget::build_third()
{ // Adjust simulation elements
  auto*  sim_lo = new QGridLayout(p_sim_grbx);
  auto*  th_lbl = new QLabel(tr("Number of &threads"), this);
  th_lbl->setBuddy(p_th_input);
  auto*  ev_lbl = new QLabel(tr("Number of &events"), this);
  ev_lbl->setBuddy(p_ev_input);
  p_th_input->setRange(1,QThread::idealThreadCount());
  p_ev_input->setRange(min_evnum,max_evnum);
  sim_lo->addWidget(th_lbl,     0, 0);
  sim_lo->addWidget(p_th_input, 0, 1);
  sim_lo->addWidget(ev_lbl,     1, 0);
  sim_lo->addWidget(p_ev_input, 1, 1);
}

void Input_Widget::build_fourth()
{ // Adjust controls
  auto*  ctrl_lo = new QGridLayout(p_ctrl_grbx);
  p_stop_btn->setDisabled(true);
  p_pause_btn->setDisabled(true);
  ctrl_lo->addWidget(p_stop_btn,  0, 0);
  ctrl_lo->addWidget(p_pause_btn, 0, 1);
  ctrl_lo->addWidget(p_start_btn, 0, 2);
}

void Input_Widget::paintEvent(QPaintEvent *event)
{
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void Input_Widget::update()
{
  p_par_desc->setText(QString(tr("[%1;\t%2]"))
                      .arg(QString().setNum(p_mu_input->value() * p_ch_input->value() * min_load, 'g', 4))
                      .arg(QString().setNum(p_mu_input->value() * p_ch_input->value() * max_load, 'g', 4)));
  p_avg_desc->setText(QString(tr("[%1;\t%2]"))
                      .arg(QString().setNum(1/(p_mu_input->value() * p_ch_input->value() * max_load), 'g', 4))
                      .arg(QString().setNum(1/(p_mu_input->value() * p_ch_input->value() * min_load), 'g', 4)));
}

void Input_Widget::load_data()
{
  p_system_parameters->set({min_load, max_load}, p_mu_input->value(),
                           p_ch_input->value(), p_pr_input->value(),
                           p_th_input->value(), p_ev_input->value());
}

void Input_Widget::slot_done()
{
  p_start_btn->setDisabled(false);
  p_pause_btn->setDisabled(true);
  p_stop_btn->setDisabled(true);
  p_system_parameters->set({min_load, max_load}, p_mu_input->value(),
                           p_ch_input->value(), p_pr_input->value(),
                           p_th_input->value(), p_ev_input->value());
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
  if( (m_paused = (!m_paused)) )
    {
      p_pause_btn->setText(tr("Resume"));
      emit signal_pause();
    }
  else
    {
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
  emit signal_stop();
}
