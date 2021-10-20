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

  enum
  {
    MENU_BUTTONS = 3,
    MENUS = 6
  };

public:
  explicit Graphs_Switch(QWidget *parent = nullptr);
  ~Graphs_Switch();

protected slots:
  void slot_approximation();
  void slot_save();
  void slot_update(int);

signals:
  void signal_show(int);
  void signal_approximate(int);
  void signal_deapproximate(int);
  void signal_save(QString);
  void signal_zoom_in();
  void signal_zoom_out();
  void signal_zoom_reset();

private:
  std::array<QPushButton*, MENU_BUTTONS> m_vector_buttons;
  std::array<QMenu*, MENUS>      m_vector_menus;
  QComboBox*            p_prior_cbox;
  QPushButton*          p_zoom_in;
  QPushButton*          p_zoom_out;
  QPushButton*          p_zoom_reset;
  int                   m_button_state{0};

private:
  void create_menus();
  void adjust_widget();

};

#endif // GRAPHS_SWITCH_H
