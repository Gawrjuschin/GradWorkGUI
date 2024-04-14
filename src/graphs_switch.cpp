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
    : QWidget(parent),
      m_vector_buttons{new QPushButton(tr("W_0(L)"),
                                       this), // Тут лучше другой текст сделать,
                                              // а ещё лучше картинку в CSS
                       new QPushButton(tr("U_0(L)"), this),
                       new QPushButton(tr("Z_0(L)"), this)},
      p_prior_cbox(new QComboBox(this)), p_zoom_in(new QPushButton(this)),
      p_zoom_out(new QPushButton(this)), p_zoom_reset(new QPushButton(this)) {
  create_menus();
  adjust_widget();

  connect(p_prior_cbox,
	  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
	  [this](int priority) {
	    m_button_state = m_button_state / 2 * 2 + priority;
            emit sigShow(m_button_state);
	  });
  
  connect(p_zoom_in, &QPushButton::clicked, this, &Graphs_Switch::sigZoomIn);
  connect(p_zoom_out, &QPushButton::clicked, this, &Graphs_Switch::sigZoomOut);
  connect(p_zoom_reset, &QPushButton::clicked, this, &Graphs_Switch::sigZoomReset);
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

  for (int i = 0; i < m_vector_buttons.size(); ++i) {
    chart_switch_lo->addWidget(m_vector_buttons[i], 0, i, 2, 1, Qt::AlignCenter);
    m_vector_buttons[i]->setMenu(m_menus_array[i * 2]);
    m_vector_buttons[i]->setFixedSize({button_width, button_height});
  }
}

void Graphs_Switch::create_menus()
{
  for (std::size_t i = 0; i < m_menus_array.size(); ++i) {
    m_menus_array[i] = new QMenu;
    auto* act = m_menus_array[i]->addAction(tr("Show approximation"));
    act->setCheckable(true);
    connect(act, &QAction::triggered, this, &Graphs_Switch::onApproximation);
    act = m_menus_array[i]->addAction(tr("Save"));
    connect(act, &QAction::triggered, this, &Graphs_Switch::onSave);
    connect(m_menus_array[i], &QMenu::aboutToShow, [this, i] {
      m_button_state = i;
      p_prior_cbox->setCurrentIndex(0);
      emit sigShow(m_button_state);
    });
  }
}

Graphs_Switch::~Graphs_Switch() = default;

void Graphs_Switch::onApproximation()
{
  if (m_menus_array[m_button_state]->actions()[0]->isChecked()) {
    emit sigApproximate(m_button_state);
  } else {
    emit sigDeapproximate(m_button_state);
  }
}

void Graphs_Switch::onUpdate(int chbox_state)
{
  for (std::size_t i = 0; i < m_vector_buttons.size(); ++i) // Обновляем кнопки
  {
    m_vector_buttons[i]->setMenu(m_menus_array[i * 2 + chbox_state]);
    m_vector_buttons[i]->setText(QString(tr("%1_%2(L)"))
                                     .arg("WUZ"[i])
                                     .arg(p_prior_cbox->currentIndex()));
  }
}

void Graphs_Switch::onSave()
{
  emit sigSave(QString(tr("%1_%2(L)"))
                   .arg("WUZ"[m_button_state / 2])
                   .arg(p_prior_cbox->currentIndex()));
}
