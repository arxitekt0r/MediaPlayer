#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QFile>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/ico/icons/sound.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
