#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class InputWidget;
class Results_Widget;
class Status_Bar;

class SimulationWorker;
class BackendObject;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  InputWidget* p_input;
  SimulationWorker* p_worker;
  BackendObject* p_backend;
  Results_Widget* p_results;
  Status_Bar* p_status;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void adjustCentralWidget();
  void connectComponents();
  void applyStylesheet(QString filename);
};

#endif // MAIN_WINDOW_H
