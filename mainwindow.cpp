#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transitions.h"

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
   QState * powerTimerState     = new QState(parentState);
   QState * modeSelectState     = new QState(parentState);
   QState * modeTimerState      = new QState(parentState);
   QState * timerSelectState    = new QState(parentState);
   QState * defrostState        = new QState(parentState);
   QState * cookingState        = new QState(parentState);

   /* Slots to be called when entering states */
   QObject::connect(idleState, SIGNAL(entered()), this, SLOT(printIdle()));
   QObject::connect(modeSelectState, SIGNAL(entered()), this, SLOT(printMode()));
   QObject::connect(modeTimerState, SIGNAL(entered()), this, SLOT(printModeTimer()));
   QObject::connect(cookingState, SIGNAL(entered()), this, SLOT(printCooking()));
   QObject::connect(powerSelectState, SIGNAL(entered()), this, SLOT(printPower()));
   QObject::connect(powerTimerState, SIGNAL(entered()), this, SLOT(printPowerTimer()));

   /* Stop transition for parent state */
   addTrans(parentState, idleState, ui->stopButton, SIGNAL(clicked()));

   /* Mode select transitions */
   addTrans(idleState, modeSelectState, ui->modeButton, SIGNAL(clicked()));
   addTrans(modeSelectState, modeTimerState, ui->modeButton, SIGNAL(clicked()));
   addTrans(modeTimerState, cookingState, ui->modeButton, SIGNAL(clicked()));

   /* Power select transitions */
   addTrans(idleState, powerSelectState, ui->powerButton, SIGNAL(clicked()));
   addTrans(powerSelectState, powerTimerState, ui->powerButton, SIGNAL(clicked()));
   addTrans(powerTimerState, cookingState, ui->powerButton, SIGNAL(clicked()));

   stateMachine->addState(parentState);
   stateMachine->setInitialState(parentState);
   parentState->setInitialState(idleState);
   stateMachine->start();
}

void MainWindow::printIdle() {
    ui->screen->setText("Idle");
}

void MainWindow::printMode() {
    ui->screen->setText("Mode");
}

void MainWindow::printModeTimer() {
    ui->screen->setText("Timer from mode");
}

void MainWindow::printPower() {
    ui->screen->setText("Power");
}

void MainWindow::printPowerTimer() {
    ui->screen->setText("Timer from power");
}

void MainWindow::printCooking() {
    ui->screen->setText("Cooking");
}
