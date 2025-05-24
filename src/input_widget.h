#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include "input_data.h"

#include <QWidget>

class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;

/**
 * @brief The InputWidget class - виджет ввода параметров СМО и управления
 * симуляцией.
 */
class InputWidget : public QWidget {
  Q_OBJECT

  bool m_paused{false};
  double m_min_load;
  double m_max_load;
  InputData m_data{};

  QLabel* p_lambda_range;
  QDoubleSpinBox* p_mu_input;
  QSpinBox* p_ch_input;
  QDoubleSpinBox* p_pr_input;
  QSpinBox* p_threads_input;
  QSpinBox* p_events_input;
  QPushButton* p_start_btn;
  QPushButton* p_pause_btn;
  QPushButton* p_stop_btn;

public:
  explicit InputWidget(double min_load, double max_load,
                       QWidget* parent = nullptr);

  ~InputWidget();

  /**
   * @brief inputData - геттер данных формы. Используется получателем сигнала
   * InputWidget::sigStart
   * @return
   */
  const InputData& inputData() const noexcept;

public slots:
  /**
   * @brief onDone - слот, разблокирующий ввод
   */
  void onDone();

protected slots:
  /**
   * @brief onUpdate
   */
  void onUpdate();
  /**
   * @brief onStart - слот кнопки p_start_btn
   */
  void onStart();
  /**
   * @brief onPauseResume - слот кнопки p_pause_btn
   */
  void onPauseResume();
  /**
   * @brief onStop - слот кнопки p_stop_btn
   */
  void onStop();

signals:
  /**
   * @brief sigStart - сигнал о старте симуляции
   */
  void sigStart();
  /**
   * @brief sigPause - сигнал паузы симуляции
   */
  void sigPause();
  /**
   * @brief sigResume - сигнал продолжения симуляции
   */
  void sigResume();
  /**
   * @brief sigStop - сигнал остановки симуляции
   */
  void sigStop();

private:
  QWidget* makeSystemGrpbx();
  QWidget* makeSimulationGrpbx();
  QWidget* makeControlsGrpbx();

  void loadData(); // Обновляет данные. Вызывается до отправки signal_start
};

#endif // INPUT_WIDGET_H
