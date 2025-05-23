#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class InputWidget;
class ResultsWidget;
class StatusBar;

class QueSysSimulation;
class SimulationControl;

/**
 * @brief The MainWindow class - виджет главного окна приложения.
 * Главное окно состоит из виджета ввода (InputWidget), виджета результатов
 * (ResultsWidget) и статуса (StatusBar).
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);

  ~MainWindow();

private:
  InputWidget* p_input;
  QueSysSimulation* p_worker;
  SimulationControl* p_backend;
  ResultsWidget* p_results;
  StatusBar* p_status;

protected:
  /**
   * @brief closeEvent - обработка закрития окна для вызова модального диалога с
   * подтверждением.
   * @param event
   */
  void closeEvent(QCloseEvent* event) override;

private:
  void adjustCentralWidget();
  void connectComponents();
  void applyStylesheet(QString filename);
};

#endif // MAIN_WINDOW_H
