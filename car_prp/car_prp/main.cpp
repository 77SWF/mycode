#include "mainwindow.h"
#include <QApplication>
#include "homepage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    homepage home;
    home.show();
    return a.exec();
}
