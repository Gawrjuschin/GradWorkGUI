#include "main_window.h"

#include <QApplication>
#include <QLocale>
#include <QPushButton>
#include <QTranslator>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString& locale : uiLanguages) {
    const QString baseName = "gradwork_gui" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      QApplication::installTranslator(&translator);
      break;
    }
  }

  MainWindow widget;
  widget.show();

  return a.exec();
}
