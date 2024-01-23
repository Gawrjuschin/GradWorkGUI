#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class Input_Widget;
class Results_Widget;
class Status_Bar;
class Backend_Object;

class Main_Window : public QMainWindow
{
  Q_OBJECT

public:
  Main_Window(QWidget* parent = nullptr);
  ~Main_Window();

private:
  Input_Widget* p_input;
  Backend_Object* p_backend;
  QThread* p_backend_thread;
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
