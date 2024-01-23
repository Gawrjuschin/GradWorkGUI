#include "graphs_switch.h"

#include <QAction>
#include <QBrush>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QPalette>
#include <QPushButton>
#include <QSizePolicy>

constexpr int button_width = 64;
constexpr int button_height = 52;

Graphs_Switch::Graphs_Switch(QWidget* parent)
    : QWidget(parent)
    , m_vector_buttons{new QPushButton(tr("W_0(λ)"),
				       this), //Тут лучше другой текст сделать, а ещё лучше картинку в CSS
		       new QPushButton(tr("U_0(λ)"), this),
		       new QPushButton(tr("Z_0(λ)"), this)}
    , p_prior_cbox(new QComboBox(this))
    , p_zoom_in(new QPushButton(this))
    , p_zoom_out(new QPushButton(this))
    , p_zoom_reset(new QPushButton(this))
{
  create_menus();
  adjust_widget();

  connect(p_prior_cbox,
	  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	  [this](int priority) {
	    m_button_state = m_button_state / 2 * 2 + priority;
	    emit signal_show(m_button_state);
	  });

  connect(p_zoom_in, &QPushButton::clicked, this, &Graphs_Switch::signal_zoom_in);
  connect(p_zoom_out, &QPushButton::clicked, this, &Graphs_Switch::signal_zoom_out);
  connect(p_zoom_reset, &QPushButton::clicked, this, &Graphs_Switch::signal_zoom_reset);
}

void Graphs_Switch::adjust_widget()
{
  auto* main_lo = new QHBoxLayout(this);
  main_lo->setAlignment(Qt::AlignCenter);

  auto* chart_switch_gb = new QGroupBox(tr("Graphs selection"), this);
  chart_switch_gb->setAlignment(Qt::AlignCenter);
  main_lo->addWidget(chart_switch_gb);
  auto* chart_zoom_gb = new QGroupBox(tr("Graphs zoom"), this);
  chart_zoom_gb->setAlignment(Qt::AlignCenter);
  main_lo->addWidget(chart_zoom_gb);

  auto* chart_switch_lo = new QGridLayout(chart_switch_gb);
  chart_switch_lo->setRowStretch(0, 1);
  chart_switch_lo->setRowStretch(1, 1);

  chart_switch_lo->addWidget(new QLabel(tr("Priority:"), this), 0, 3, Qt::AlignCenter);
  p_prior_cbox->setFixedWidth(button_width);
  chart_switch_lo->addWidget(p_prior_cbox, 1, 3, Qt::AlignCenter);
  p_prior_cbox->addItems({tr("First"), tr("Second")});

  auto* chart_zoom_lo = new QHBoxLayout(chart_zoom_gb);
  p_zoom_in->setFixedSize({button_height, button_height});
  p_zoom_in->setObjectName("zoomInBtn");
  p_zoom_in->setText(tr("+"));
  p_zoom_out->setFixedSize({button_height, button_height});
  p_zoom_out->setObjectName("zoomOutBtn");
  p_zoom_out->setText(tr("-"));
  p_zoom_reset->setFixedSize({button_height, button_height});
  p_zoom_reset->setObjectName("zoomResetBtn");
  p_zoom_reset->setText(tr("X"));
  chart_zoom_lo->addWidget(p_zoom_in);
  chart_zoom_lo->addWidget(p_zoom_out);
  chart_zoom_lo->addWidget(p_zoom_reset);

  for (std::size_t i = 0; i < m_vector_buttons.size(); ++i) {
    chart_switch_lo->addWidget(m_vector_buttons[i], 0, i, 2, 1, Qt::AlignCenter);
    m_vector_buttons[i]->setMenu(m_vector_menus[i * 2]);
    m_vector_buttons[i]->setFixedSize({button_width, button_height});
  }
}

void Graphs_Switch::create_menus()
{
  for (std::size_t i = 0; i < m_vector_menus.size(); ++i) {
    m_vector_menus[i] = new QMenu;
    auto* act = m_vector_menus[i]->addAction(tr("Show approximation"));
    act->setCheckable(true);
    connect(act, &QAction::triggered, this, &Graphs_Switch::slot_approximation);
    act = m_vector_menus[i]->addAction(tr("Save"));
    connect(act, &QAction::triggered, this, &Graphs_Switch::slot_save);
    connect(m_vector_menus[i], &QMenu::aboutToShow, [=] {
      m_button_state = i;
      p_prior_cbox->setCurrentIndex(0);
      emit signal_show(m_button_state);
    });
  }
}

Graphs_Switch::~Graphs_Switch() = default;

void Graphs_Switch::slot_approximation()
{
  if (m_vector_menus[m_button_state]->actions()[0]->isChecked()) {
    emit signal_approximate(m_button_state);
  } else {
    emit signal_deapproximate(m_button_state);
  }
}

void Graphs_Switch::slot_update(int chbox_state)
{
  for (std::size_t i = 0; i < m_vector_buttons.size(); ++i) // Обновляем кнопки
  {
    m_vector_buttons[i]->setMenu(m_vector_menus[i * 2 + chbox_state]);
    m_vector_buttons[i]->setText(
	QString(tr("%1_%2(λ)")).arg("WUZ"[i]).arg(p_prior_cbox->currentIndex()));
  }
}

void Graphs_Switch::slot_save()
{
  emit signal_save(
      QString(tr("%1_%2(λ)")).arg("WUZ"[m_button_state / 2]).arg(p_prior_cbox->currentIndex()));
}
