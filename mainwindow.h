#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStateMachine>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void printIdle();
    void printMode();
    void printModeTimer();
    void printCooking();
    void printPower();
    void printPowerTimer();
    void printDefrost();
    void printHour();
    void printMinute();
    void updateDial(int i);

private:
    Ui::MainWindow *ui;
    QStateMachine * stateMachine;
    QState * parentState;
    QState * idleState;
    QState * minuteSettingState;
    QState * hourSettingState;
    QState * powerSelectState;
    QState * powerTimerState;
    QState * modeSelectState;
    QState * modeTimerState;
    QState * timerSelectState;
    QState * defrostState;
    QState * cookingState;

    void setUpStateChart(); // Sets up states and transitions
    void setUpDial(); // Sets up the dial actions
    inline QString addZeroes(int i);
    int timer;
    QString currentMode;
    int currentPower;
    int hour;
    int minute;
};

#endif // MAINWINDOW_H
