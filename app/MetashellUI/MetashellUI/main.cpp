#include "metashellmainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  MetashellMainWindow w;
  w.show();

  return a.exec();
}
