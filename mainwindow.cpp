#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transitions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stateMachine = new QStateMachine(this);
    setUpStateChart();
    setUpDial();
    timer = 60;
    currentMode = "Microwaves";
    currentPower = 0;
    hour = 0;
    minute = 0;
    setUpCookingTimer();
    setUpClockTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stateMachine;
    delete parentState;
    delete idleState;
    delete minuteSettingState;
    delete hourSettingState;
    delete powerSelectState;
    delete powerTimerState;
    delete modeSelectState;
    delete modeTimerState;
    delete timerSelectState;
    delete defrostState;
    delete cookingState;
    delete cookingTimer;
    delete clockTimer;
}


void MainWindow::setUpStateChart()
{
   parentState         = new QState(stateMachine); // Contains the stop behavior for all
   idleState           = new QState(parentState);
   minuteSettingState  = new QState(parentState);
   hourSettingState    = new QState(parentState);
   powerSelectState    = new QState(parentState);
   powerTimerState     = new QState(parentState);
   modeSelectState     = new QState(parentState);
   modeTimerState      = new QState(parentState);
   timerSelectState    = new QState(parentState);
   defrostState        = new QState(parentState);
   cookingState        = new QState(parentState);

   /* Slots to be called when entering states */
   QObject::connect(idleState, SIGNAL(entered()), this, SLOT(printIdle()));
   QObject::connect(modeSelectState, SIGNAL(entered()), this, SLOT(printMode()));
   QObject::connect(modeTimerState, SIGNAL(entered()), this, SLOT(printModeTimer()));
   QObject::connect(powerSelectState, SIGNAL(entered()), this, SLOT(printPower()));
   QObject::connect(powerTimerState, SIGNAL(entered()), this, SLOT(printPowerTimer()));
   QObject::connect(defrostState, SIGNAL(entered()), this, SLOT(printDefrost()));
   QObject::connect(cookingState, SIGNAL(entered()), this, SLOT(printCooking()));
   QObject::connect(hourSettingState, SIGNAL(entered()), this, SLOT(printHour()));
   QObject::connect(minuteSettingState, SIGNAL(entered()), this, SLOT(printMinute()));

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

   /* Defrost transitions */
   addTrans(idleState, defrostState, ui->defrostButton, SIGNAL(clicked()));
   addTrans(defrostState, cookingState, ui->defrostButton, SIGNAL(clicked()));

   /* Clock settings transitions */
   addTrans(idleState, hourSettingState, ui->clockButton, SIGNAL(clicked()));
   addTrans(hourSettingState, minuteSettingState, ui->clockButton, SIGNAL(clicked()));
   addTrans(minuteSettingState, idleState, ui->clockButton, SIGNAL(clicked()));

   /* Idle to timer transition by touching the dial */
   addTrans(idleState, timerSelectState, ui->dial, SIGNAL(valueChanged(int)));
   addTrans(timerSelectState, cookingState, ui->startButton, SIGNAL(clicked()));

   /* Transition from idle to cooking just by pressing start */
   addTrans(idleState, cookingState, ui->startButton, SIGNAL(clicked()));

   stateMachine->setInitialState(parentState);
   parentState->setInitialState(idleState);
   stateMachine->start();
}

void MainWindow::setUpCookingTimer()
{
    cookingTimer = new QTimer();
    /* Timeout for cooking timer is one second */
    cookingTimer->setInterval(1000);
    QObject::connect(cookingTimer, SIGNAL(timeout()), this, SLOT(updateCookingTimer()));
}

void MainWindow::setUpClockTimer()
{
    clockTimer = new QTimer();
    /* Timeout for cooking timer is one minute */
    clockTimer->setInterval(60 * 1000);
    QObject::connect(clockTimer, SIGNAL(timeout()), this, SLOT(updateClockTimer()));
    clockTimer->start();
}

void MainWindow::setUpDial()
{
    QDial * dial = ui->dial;

    QObject::connect(dial, SIGNAL(valueChanged(int)), this, SLOT(updateDial(int)));
}

void MainWindow::printIdle() {
    ui->dial->setValue(0);
    timer = 60;
    cookingTimer->stop();
    /* Did not know how to do this properly so it's very ugly, sorry */
    QString hourString = addZeroes(hour);
    QString minuteString = addZeroes(minute);

    ui->screen->setText(hourString + " : " + minuteString);
}

void MainWindow::printMode() {
    ui->dial->setValue(0);
    ui->screen->setText("Mode : " + currentMode);
}

void MainWindow::printModeTimer() {
    ui->dial->setValue(0);
    timer = ui->dial->value();
    ui->screen->setText(QString::number(timer));
}

void MainWindow::printPower() {
    ui->dial->setValue(0);
    currentPower = 0;
    ui->screen->setText("Power : " + QString::number(currentPower) + "%");
}

void MainWindow::printPowerTimer() {
    ui->dial->setValue(0);
    updateDial(ui->dial->value());
    ui->screen->setText(QString::number(timer));
}

void MainWindow::printDefrost() {
    ui->dial->setValue(0);
    timer = ui->dial->value();
    ui->screen->setText("Weight : " + QString::number(50 * timer) + " g");
}

void MainWindow::printCooking() {
    cookingTimer->start();
    ui->screen->setText("Cooking, timer is " + QString::number(timer));
}

void MainWindow::printHour() {
    ui->dial->setValue(99 * hour/23);
    QString hourString = addZeroes(hour);
    ui->screen->setText("Setting hour : " + hourString);
}

void MainWindow::printMinute() {
    ui->dial->setValue(99 * minute/59);
    QString minuteString = addZeroes(minute);
    ui->screen->setText("Setting minute : " + minuteString);
}

void MainWindow::updateDial(int i) {
    /* States where the dial updates the timer */
    if (stateMachine->configuration().contains(timerSelectState) |
        stateMachine->configuration().contains(powerTimerState)  |
        stateMachine->configuration().contains(modeTimerState)) {
        timer = i;
        ui->screen->setText(QString::number(timer));
    }

    /* State where the dial updates the mode selection */
    if (stateMachine->configuration().contains(modeSelectState)) {
        switch(ui->dial->value()/25) {
        case 0:
            currentMode = "Microwaves";
            break;
        case 1:
            currentMode = "Grill";
            break;
        case 2:
            currentMode = "Grill & microwaves";
            break;
        case 3:
            currentMode = "Je fais le café aussi ?";
        }
        ui->screen->setText("Mode : " + currentMode);
    }

    /* State where the dial updates the power level */
    if (stateMachine->configuration().contains(powerSelectState)) {
        currentPower = ui->dial->value() + 1;
        if (currentPower < 100)
            ui->screen->setText("Power : " + QString::number(currentPower) + " %");
        else
            ui->screen->setText("IT'S OVER NINE THOUSAND !");
    }

    /* State where the dial updates the weight of food to defrost */
    if (stateMachine->configuration().contains(defrostState)) {
        timer = ui->dial->value() * 10;
        ui->screen->setText("Weight : " + QString::number(5 * timer) + " g");
    }

    /* State where the dial updates the hours */
    if (stateMachine->configuration().contains(hourSettingState)) {
        hour = ui->dial->value() * 24/99;
        if (hour == 24)
            hour --;
        QString hourString = addZeroes(hour);
        ui->screen->setText("Setting hour : " + hourString);
    }

    /* State where the dial updates the hours */
    if (stateMachine->configuration().contains(minuteSettingState)) {
        minute = ui->dial->value() * 60/99;
        if (minute == 60)
            minute --;
        QString minuteString = addZeroes(minute);
        ui->screen->setText("Setting minute : " + minuteString);
    }
}

inline QString MainWindow::addZeroes(int i) {
    QString str = QString::number(i);
    if (i < 10)
        str = "0" + str;

    return str;
}

void MainWindow::updateCookingTimer()
{
    timer --;
    if (timer > 0)
        ui->screen->setText("Cooking, timer is " + QString::number(timer));
    else {
        cookingTimer->stop();
        ui->screen->setText("BEEP BEEP BEEP");
    }
}

void MainWindow::updateClockTimer()
{
    if (!(stateMachine->configuration().contains(hourSettingState) |
          stateMachine->configuration().contains(minuteSettingState))) {
        if (minute++ == 59) {
            minute =0;
            if (hour ++ == 23)
                hour = 0;
        }
        printIdle();
    }
}
