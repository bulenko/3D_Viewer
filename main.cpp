#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  setlocale(LC_NUMERIC, "C");
  w.show();
  return a.exec();
}
