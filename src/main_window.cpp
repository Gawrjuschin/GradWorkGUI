#include "main_window.h"
#include "backend_object.h"
#include "graphs_data.h"
#include "input_widget.h"
#include "results_widget.h"
#include "status_bar.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QLayout>
#include <QMessageBox>
#include <QStatusBar>

#include <QThread>

constexpr int input_width = 280;
constexpr int progressbar_margins = 4; // Где-то зашиты отступы в 2px
constexpr auto stylesheet_reference = ":/style.qss";

Main_Window::Main_Window(QWidget* parent)
    : QMainWindow(parent)
    , p_input(new Input_Widget)
    , p_backend(new Backend_Object(p_input->data()))
    , p_backend_thread(new QThread(this))
{
  setWindowTitle(tr("Prioritized system simulation"));
  adjustCentralWidget();
  p_status = new Status_Bar(p_backend->getProgress(), this);
  p_status->setProgressBarWidth(input_width - progressbar_margins);
  setStatusBar(p_status);
  p_status->setMaximum(Graphs_Data::POINTS_COUNT);
  connectComponents();

  p_backend->moveToThread(p_backend_thread);
  p_backend_thread->start();
  //  applyStylesheet(QString::fromLatin1(stylesheet_reference));
}
Main_Window::~Main_Window()
{
  p_backend->slot_stop();
  while (p_backend->getProgress().value()) {
    QApplication::processEvents();
  }
  p_backend_thread->exit();
  p_backend_thread->wait();
}

void Main_Window::adjustCentralWidget()
{
  auto* central_widget = new QWidget(this);
  auto* lo = new QHBoxLayout(central_widget);
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
  connect(p_input, &Input_Widget::signal_start, p_results, &Results_Widget::slot_start);
  connect(p_input, &Input_Widget::signal_start, p_backend, &Backend_Object::slot_start);
  connect(p_input, &Input_Widget::signal_pause, p_results, &Results_Widget::slot_pause);
  connect(p_input, &Input_Widget::signal_pause, p_backend, &Backend_Object::slot_pause);
  connect(p_input, &Input_Widget::signal_resume, p_results, &Results_Widget::slot_resume);
  connect(p_input, &Input_Widget::signal_resume, p_backend, &Backend_Object::slot_resume);
  connect(p_input, &Input_Widget::sigStop, p_results, &Results_Widget::slot_stop);
  connect(p_input, &Input_Widget::sigStop, p_backend, &Backend_Object::slot_stop);
  connect(p_backend, &Backend_Object::signal_done, p_results, &Results_Widget::slot_done);
  connect(p_results, &Results_Widget::signal_ready, p_input, &Input_Widget::slot_done);

  connect(p_input, &Input_Widget::signal_start, p_status, &Status_Bar::slot_start);
  connect(p_input, &Input_Widget::sigStop, p_status, &Status_Bar::onStop);
  connect(p_backend, &Backend_Object::signal_done, p_status, &Status_Bar::onReady);
}

void Main_Window::closeEvent(QCloseEvent* event)
{
  QMessageBox::StandardButton resBtn = QMessageBox::question(this,
							     QCoreApplication::applicationName(),
							     tr("Are you sure?\n"),
							     QMessageBox::No | QMessageBox::Yes,
							     QMessageBox::Yes);
  if (resBtn == QMessageBox::Yes) {
    // Здесь нужно останавливать активные треды
    event->accept();
  } else {
    event->ignore();
  }
}

void Main_Window::applyStylesheet(QString filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) { // Проверка qrc на всякий случай
    QMessageBox::StandardButton reply{};
    reply = QMessageBox::information(this, tr("Error!"), tr("Cannot load default style!"));
    if (reply == QMessageBox::Ok) {
    };
    return;
  }
  QString stylesheet = QString::fromUtf8(file.readAll());
  qApp->setStyleSheet(stylesheet);
}
