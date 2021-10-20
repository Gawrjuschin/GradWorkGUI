#include "main_window.h"
#include "Backend/backend_object.h"
#include "Backend/graphs_data.h"
#include "Widgets/input_widget.h"
#include "Widgets/results_widget.h"
#include "Widgets/status_bar.h"

#include <QApplication>
#include <QCloseEvent>
#include <QLayout>
#include <QStatusBar>
#include <QFile>
#include <QMessageBox>

constexpr int input_width = 280;
constexpr int progressbar_margins = 4; // Где-то зашиты отступы в 2px
constexpr auto stylesheet_reference = ":/style.qss";

Main_Window::Main_Window(QWidget *parent)
  : QMainWindow(parent)
  , p_backend(new Backend_Object(this))
{
  setWindowTitle(tr("Prioritized system simulation"));
  adjustCentralWidget();
  p_status = new Status_Bar(p_backend->getProgress(), this);
  p_status->setProgressBarWidth(input_width - progressbar_margins);
  setStatusBar(p_status);
  p_status->setMaximum(Graphs_Data::POINTS_COUNT);
  connectComponents();
  applyStylesheet(QString::fromLatin1(stylesheet_reference));
}

void Main_Window::adjustCentralWidget()
{
  auto* central_widget = new QWidget(this);
  auto* lo = new QHBoxLayout(central_widget);
  p_input = new Input_Widget(p_backend->system_data());
  p_input->setFixedWidth(input_width);
  p_results = new Results_Widget(p_backend->table_data(), p_backend->graphs_data());
  lo->setContentsMargins({0, 0, 0, 0});
  lo->setSpacing(0);
  lo->addWidget(p_input);
  lo->addWidget(p_results, 1);
  setCentralWidget(central_widget);
  setContentsMargins({0, 0, 0, 0});
}

void Main_Window::connectComponents()
{
  connect(p_input,   &Input_Widget::signal_start,  p_results,   &Results_Widget::slot_start);
  connect(p_input,   &Input_Widget::signal_start,  p_backend,   &Backend_Object::slot_start);
  connect(p_input,   &Input_Widget::signal_pause,  p_results,   &Results_Widget::slot_pause);
  connect(p_input,   &Input_Widget::signal_pause,  p_backend,   &Backend_Object::slot_pause);
  connect(p_input,   &Input_Widget::signal_resume, p_results,   &Results_Widget::slot_resume);
  connect(p_input,   &Input_Widget::signal_resume, p_backend,   &Backend_Object::slot_resume);
  connect(p_input,   &Input_Widget::signal_stop,   p_results,   &Results_Widget::slot_stop);
  connect(p_input,   &Input_Widget::signal_stop,   p_backend,   &Backend_Object::slot_stop);
  connect(p_backend, &Backend_Object::signal_done, p_results,   &Results_Widget::slot_done);
  connect(p_results, &Results_Widget::signal_ready,p_input,     &Input_Widget::slot_done);

  connect(p_input,   &Input_Widget::signal_start, p_status, &Status_Bar::slot_start);
  connect(p_input,   &Input_Widget::signal_stop,  p_status, &Status_Bar::slot_reset);
  connect(p_backend, &Backend_Object::signal_done,  p_status, &Status_Bar::slot_stop);

}

Main_Window::~Main_Window() = default;

void Main_Window::closeEvent(QCloseEvent *event)
{
  QMessageBox::StandardButton resBtn = QMessageBox::question( this, QCoreApplication::applicationName(),
                                                              tr("Are you sure?\n"),
                                                              QMessageBox::No | QMessageBox::Yes,
                                                              QMessageBox::Yes);
  if (resBtn == QMessageBox::Yes)
  {
      // Здесь нужно останавливать активные треды
      event->accept();
  }
  else
  {
      event->ignore();
  }
}

void Main_Window::applyStylesheet(QString filename)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly))
    { // Проверка qrc на всякий случай
      QMessageBox::StandardButton reply{};
      reply = QMessageBox::information(this, tr("Error!"), tr("Cannot load default style!"));
      if (reply == QMessageBox::Ok) { };
      return;
    }
  QString stylesheet = QString::fromUtf8(file.readAll());
  qApp->setStyleSheet(stylesheet);
}
