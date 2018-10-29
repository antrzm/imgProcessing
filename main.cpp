#include <QCoreApplication>
#include <QApplication>

#include <QtWidgets/QWidget>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
