#include "main_window.h"
#include "backendobject.h"
#include "input_widget.h"
#include "points_data.h"
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
// constexpr auto stylesheet_reference = ":/style.qss";

Main_Window::Main_Window(QWidget* parent)
    : QMainWindow(parent),
      p_input(new Input_Widget(PointsData::kMinLoad, PointsData::kMaxLoad)),
      p_worker(new SimulationWorker(p_input->data())),
      p_backend(new BackendObject(p_worker, this)),
      p_results(
          new Results_Widget(p_worker->TableData(), p_worker->PointsData())),
      p_status(new Status_Bar(p_backend->getProgress(), this)) {
  setWindowTitle(tr("Prioritized system simulation"));
  adjustCentralWidget();
  p_status->setProgressBarWidth(input_width - progressbar_margins);
  setStatusBar(p_status);
  p_status->setMaximum(PointsData::kPointsCount);
  connectComponents();

  //  applyStylesheet(QString::fromLatin1(stylesheet_reference));
}
Main_Window::~Main_Window() = default;

void Main_Window::adjustCentralWidget()
{
  auto* central_widget = new QWidget(this);
  auto* lo = new QHBoxLayout(central_widget);
  p_input->setFixedWidth(input_width);
  lo->setContentsMargins({0, 0, 0, 0});
  lo->setSpacing(0);
  lo->addWidget(p_input);
  lo->addWidget(p_results, 1);
  setCentralWidget(central_widget);
  setContentsMargins({0, 0, 0, 0});
}

void Main_Window::connectComponents()
{
  connect(p_input, &Input_Widget::sigStart, p_results,
          &Results_Widget::onStart);
  connect(p_input, &Input_Widget::sigStart, p_backend, &BackendObject::onStart);
  connect(p_input, &Input_Widget::sigPause, p_results,
          &Results_Widget::onPause);
  connect(p_input, &Input_Widget::sigPause, p_backend, &BackendObject::onPause);
  connect(p_input, &Input_Widget::sigResume, p_results,
          &Results_Widget::onResume);
  connect(p_input, &Input_Widget::sigResume, p_backend,
          &BackendObject::onResume);
  connect(p_input, &Input_Widget::sigStop, p_results, &Results_Widget::onStop);
  connect(p_input, &Input_Widget::sigStop, p_backend, &BackendObject::onStop);
  connect(p_backend, &BackendObject::sigDataReady, p_results,
          &Results_Widget::onDataReady);
  connect(p_results, &Results_Widget::sigDataReady, p_input,
          &Input_Widget::onDone);

  connect(p_input, &Input_Widget::sigStart, p_status, &Status_Bar::onStart);
  connect(p_input, &Input_Widget::sigStop, p_status, &Status_Bar::onStop);
  connect(p_backend, &BackendObject::sigDataReady, p_status,
          &Status_Bar::onReady);
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
