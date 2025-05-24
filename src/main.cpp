#include "main_window.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  QApplication::setStyle(QStyleFactory::create("Fusion"));

  MainWindow widget;
  widget.show();

  return a.exec();
}
