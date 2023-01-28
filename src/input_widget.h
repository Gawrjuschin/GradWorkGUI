#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include <QWidget>

class QGroupBox;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QLabel;
struct System_Data;

class Input_Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Input_Widget( std::shared_ptr<System_Data> _system_parameters, QWidget *parent = nullptr);
  ~Input_Widget();

protected:
  void paintEvent(QPaintEvent* event) override;

public slots:
  void slot_done();

protected slots:
  void update();
  void slot_start();
  void slot_pause_resume();
  void slot_stop();

signals:
  void signal_start();
  void signal_pause();
  void signal_resume();
  void signal_stop();

private:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  QGroupBox*      p_sys_grbx;
  QGroupBox*      p_sim_grbx;
  QGroupBox*      p_ctrl_grbx;
  QLabel*         p_par_desc;
  QLabel*         p_avg_desc;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  QDoubleSpinBox* p_mu_input;
  QSpinBox*       p_ch_input;
  QDoubleSpinBox* p_pr_input;
  QSpinBox*       p_th_input;
  QSpinBox*       p_ev_input;
  QPushButton*    p_stop_btn;
  QPushButton*    p_pause_btn;
  QPushButton*    p_start_btn;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  std::shared_ptr<System_Data> p_system_parameters;//Разделяемые данные, чтобы сигналом их не посылать
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //Флаг паузы. Далее будет заменён на класс-синхронизатор из стороннего проекта с синхронизацией потоков
  bool m_paused{false};

private:
  void load_data();//Обновляет данные. Вызывается до отправки signal_start
  void build_first();
  void build_second();
  void build_third();
  void build_fourth();

};

#endif // INPUT_WIDGET_H
