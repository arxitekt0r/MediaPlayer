#include "mainwindow.h"
#include "player.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setStyleSheet("QMainWindow { background-color: black; }");
    setFixedSize(600, 300);
    player = new Player(this);
}

MainWindow::~MainWindow()
{
    delete player;
}
