#ifndef INPUT_WIDGET_H
#define INPUT_WIDGET_H

#include "input_data.h"

#include <QWidget>

struct InputWidgetImpl;

class InputWidget : public QWidget {
  Q_OBJECT

public:
  explicit InputWidget(double min_load, double max_load,
                       QWidget* parent = nullptr);

  InputWidget(const InputWidget&) = delete;
  InputWidget& operator=(const InputWidget&) = delete;

  InputWidget(InputWidget&&) = delete;
  InputWidget& operator=(InputWidget&&) = delete;

  ~InputWidget();

  const InputData& inputData() const noexcept;

public slots:
  void onDone();

protected slots:
  void onUpdate();
  void onStart();
  void onPauseResume();
  // TODO: пересмотреть логику остановки по аналогии с onDone
  void onStop();

signals:
  void sigStart();
  void sigPause();
  void sigResume();
  void sigStop();

private:
  std::unique_ptr<InputWidgetImpl> p_impl_;
  InputData m_data{};

private:
  QWidget* makeSystemGrpbx();
  QWidget* makeSimulationGrpbx();
  QWidget* makeControlsGrpbx();

  void loadData(); // Обновляет данные. Вызывается до отправки signal_start
};

#endif // INPUT_WIDGET_H
