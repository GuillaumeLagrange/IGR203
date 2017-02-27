#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Transitions.h"

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


void MainWindow::setUpStateChart() {
   QState * parentState         = new QState(); // Contains the stop behavior for all
   QState * idleState           = new QState(parentState);
   QState * minuteSettingState  = new QState(parentState);
   QState * hourSettingState    = new QState(parentState);
   QState * powerSelectState    = new QState(parentState);
   QState * modeSelectState     = new QState(parentState);
   QState * timerSelectState    = new QState(parentState);
   QState * defrostState        = new QState(parentState);

   QObject::connect(idleState, SIGNAL(entered()), this, SLOT(printIdle()));
   QObject::connect(modeSelectState, SIGNAL(entered()), this, SLOT(printMode()));

   addTrans(idleState, modeSelectState, ui->modeButton, SIGNAL(clicked()));

   stateMachine->addState(parentState);
   stateMachine->setInitialState(parentState);
   parentState->setInitialState(idleState);
   stateMachine->start();
}

void MainWindow::printIdle() {
    ui->screen->setText("Idle");
}

void MainWindow::printMode() {
    ui->screen->setText("Mode select");
}
