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

constexpr int kButtonWidth = 64;
constexpr int kButtonHeight = 52;

GraphsSwitch::GraphsSwitch(QWidget* parent)
    : QWidget(parent), m_graphs_buttons{new QPushButton(tr("W(L)"), this),
                                        new QPushButton(tr("U(L)"), this),
                                        new QPushButton(tr("Z(L)"), this)},
      p_prior_cbox(new QComboBox(this)) {
  auto* main_lo = new QHBoxLayout(this);
  main_lo->setAlignment(Qt::AlignCenter);

  auto* chart_switch_gb = MakeGraphsGroup();
  main_lo->addWidget(chart_switch_gb, 0);

  auto* zoom_gb = MakeZoomGroup();
  main_lo->addWidget(zoom_gb, 0);

  main_lo->addStretch(1);

  connect(
      p_prior_cbox,
      static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      this, &GraphsSwitch::onPriorityChange);
}

QGroupBox* GraphsSwitch::MakeGraphsGroup() {
  CreateMenus();

  auto* chart_switch_gb = new QGroupBox(tr("Graphs selection"), this);
  chart_switch_gb->setAlignment(Qt::AlignCenter);

  auto* chart_switch_lo = new QGridLayout(chart_switch_gb);
  chart_switch_lo->setRowStretch(0, 1);
  chart_switch_lo->setRowStretch(1, 1);

  chart_switch_lo->addWidget(new QLabel(tr("Priority:"), this), 0, 3,
                             Qt::AlignCenter);
  p_prior_cbox->setFixedWidth(kButtonWidth);
  chart_switch_lo->addWidget(p_prior_cbox, 1, 3, Qt::AlignCenter);
  p_prior_cbox->addItems({tr("First"), tr("Second")});

  for (int i = 0; i < std::size(m_graphs_buttons); ++i) {
    chart_switch_lo->addWidget(m_graphs_buttons[i], 0, i, 2, 1,
                               Qt::AlignCenter);
    m_graphs_buttons[i]->setMenu(m_graphs_menus[i * 2]);
    m_graphs_buttons[i]->setFixedSize({kButtonWidth, kButtonHeight});
  }
  return chart_switch_gb;
}

QGroupBox* GraphsSwitch::MakeZoomGroup() {
  auto* zoom_gb = new QGroupBox(tr("Graphs zoom"), this);
  zoom_gb->setAlignment(Qt::AlignCenter);

  auto* zoom_lo = new QHBoxLayout(zoom_gb);

  auto* zoom_in_btn = new QPushButton(tr("+"));
  zoom_in_btn->setFixedSize({kButtonHeight, kButtonHeight});
  zoom_in_btn->setObjectName("zoomInBtn");

  auto* zoom_out_btn = new QPushButton(tr("-"));
  zoom_out_btn->setFixedSize({kButtonHeight, kButtonHeight});
  zoom_out_btn->setObjectName("zoomOutBtn");

  auto* zoom_reset_btn = new QPushButton(tr("X"));
  zoom_reset_btn->setFixedSize({kButtonHeight, kButtonHeight});
  zoom_reset_btn->setObjectName("zoomResetBtn");

  zoom_lo->addWidget(zoom_in_btn);
  zoom_lo->addWidget(zoom_out_btn);
  zoom_lo->addWidget(zoom_reset_btn);

  connect(zoom_in_btn, &QPushButton::clicked, this, &GraphsSwitch::sigZoomIn);
  connect(zoom_out_btn, &QPushButton::clicked, this, &GraphsSwitch::sigZoomOut);
  connect(zoom_reset_btn, &QPushButton::clicked, this,
          &GraphsSwitch::sigZoomReset);

  return zoom_gb;
}

void GraphsSwitch::CreateMenus() {
  for (std::size_t i{}; i < std::size(m_graphs_menus); ++i) {
    m_graphs_menus[i] = new QMenu;

    auto* show_act = m_graphs_menus[i]->addAction(tr("Show approximation"));
    show_act->setCheckable(true);
    connect(show_act, &QAction::triggered, this,
            &GraphsSwitch::onApproximation);

    auto* save_act = m_graphs_menus[i]->addAction(tr("Save"));
    connect(save_act, &QAction::triggered, this, &GraphsSwitch::onSave);

    connect(m_graphs_menus[i], &QMenu::aboutToShow, [this, i] {
      m_selected_graph = i;
      emit sigShow(m_selected_graph);
    });
  }
}

void GraphsSwitch::reset() {
  if (m_selected_graph % 2 != 0) {
    p_prior_cbox->setCurrentIndex(0);
  }
  for (auto* menu : m_graphs_menus) {
    menu->actions()[0]->setChecked(false);
  }
  m_selected_graph = 0;
}

void GraphsSwitch::onApproximation() {
  if (m_graphs_menus[m_selected_graph]->actions()[0]->isChecked()) {
    emit sigApproximate(m_selected_graph);
  } else {
    emit sigDeapproximate(m_selected_graph);
  }
}

void GraphsSwitch::onSave() {
  const auto filename = QString(tr("%1_%2(L)"))
                            .arg("WUZ"[m_selected_graph / 2])
                            .arg(p_prior_cbox->currentIndex());
  emit sigSave(filename);
}

void GraphsSwitch::onPriorityChange(int priority) {
  for (std::size_t i{}; i < std::size(m_graphs_buttons); ++i) {
    m_graphs_buttons[i]->setMenu(m_graphs_menus[i * 2 + priority]);
  }
  const auto button_num = m_selected_graph / 2 * 2;
  m_selected_graph = button_num + priority;
  emit sigShow(m_selected_graph);
}
