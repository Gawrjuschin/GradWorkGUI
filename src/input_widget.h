#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include "input_data.h"

#include <QWidget>

class QGroupBox;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QLabel;

class Input_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Input_Widget(QWidget *parent = nullptr);
    ~Input_Widget();

    const InputData& data() const noexcept;

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
    void sigStop();

private:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    QLabel* p_parameter_desc;
    QLabel* p_requests_desc;
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
    InputData m_data{};
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //Флаг паузы. Далее будет заменён на класс-синхронизатор из стороннего проекта с синхронизацией потоков
    bool m_paused{false};

private:
    void load_data();//Обновляет данные. Вызывается до отправки signal_start

};

#endif // INPUT_WIDGET_H
