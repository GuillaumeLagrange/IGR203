#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stateMachine = new QStateMachine();
    setUpStateChart();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stateMachine;
}


MainWindow::setUpStateChart() {
}
