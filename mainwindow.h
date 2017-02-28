#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStateMachine>
#include <QTimer>

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
    /* These functions are called when the machine enters in each state */
    void printIdle();
    void printMode();
    void printModeTimer();
    void printCooking();
    void printPower();
    void printPowerTimer();
    void printDefrost();
    void printHour();
    void printMinute();

    void updateDial(int i);    // Handles dial interactions
    void updateCookingTimer(); // Handles timeouts of cookingTimer
    void updateClockTimer();   // Handles timeouts of clockTimer

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

    /* These functions are called in the constructor */
    void setUpStateChart(); // Sets up states and transitions
    void setUpDial(); // Sets up the dial actions
    void setUpCookingTimer(); // Sets up the cooking timer
    void setUpClockTimer(); // Sets up the clock timer

    inline QString addZeroes(int i); // Sets time to the right format, ugly

    /* Attributes */
    int timer;
    QString currentMode;
    int currentPower;
    int hour;
    int minute;
    QTimer * cookingTimer;
    QTimer * clockTimer;
};

#endif // MAINWINDOW_H
