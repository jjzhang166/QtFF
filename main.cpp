#include "mainwindow.h"
#include <QApplication>

//#include <test.h>

#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.show();

 //   Test t;



    return a.exec();
}
