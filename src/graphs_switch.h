#ifndef GRAPHS_SWITCH_H
#define GRAPHS_SWITCH_H

#include <QWidget>
#include <array>

class QPushButton;
class QMenu;
class QComboBox;
class QGroupBox;

class GraphsSwitch : public QWidget {
  Q_OBJECT

  static constexpr std::size_t BUTTONS_COUNT{3};
  static constexpr std::size_t GRAPHS_COUNT{6};

public:
  explicit GraphsSwitch(QWidget* parent = nullptr);
  ~GraphsSwitch() = default;

  void reset();

protected slots:
  void onApproximation();
  void onSave();
  void onPriorityChange(int);

signals:
  void sigShow(int);
  void sigApproximate(int);
  void sigDeapproximate(int);
  void sigSave(QString);
  void sigZoomIn();
  void sigZoomOut();
  void sigZoomReset();

private:
  std::array<QPushButton*, BUTTONS_COUNT> m_graphs_buttons;
  std::array<QMenu*, GRAPHS_COUNT> m_graphs_menus;
  QComboBox* p_prior_cbox;
  int m_selected_graph{0};

private:
  QGroupBox* MakeGraphsGroup();
  QGroupBox* MakeZoomGroup();
  void CreateMenus();
};

#endif // GRAPHS_SWITCH_H
