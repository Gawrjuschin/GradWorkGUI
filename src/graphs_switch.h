#ifndef GRAPHS_SWITCH_H
#define GRAPHS_SWITCH_H

#include <QWidget>
#include <array>

class QPushButton;
class QMenu;
class QComboBox;

class Graphs_Switch : public QWidget
{
  Q_OBJECT

  enum { MENU_BUTTONS = 3, MENUS = 6 };

public:
  explicit Graphs_Switch(QWidget* parent = nullptr);
  ~Graphs_Switch();

protected slots:
  void onApproximation();
  void onSave();
  void onUpdate(int);

signals:
  void sigShow(int);
  void sigApproximate(int);
  void sigDeapproximate(int);
  void sigSave(QString);
  void sigZoomIn();
  void sigZoomOut();
  void sigZoomReset();

private:
  std::array<QPushButton*, MENU_BUTTONS> m_vector_buttons;
  std::array<QMenu*, MENUS> m_menus_array;
  QComboBox* p_prior_cbox;
  QPushButton* p_zoom_in;
  QPushButton* p_zoom_out;
  QPushButton* p_zoom_reset;
  int m_button_state{0};

private:
  void create_menus();
  void adjust_widget();
};

#endif // GRAPHS_SWITCH_H
