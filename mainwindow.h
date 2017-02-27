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

private:
    Ui::MainWindow *ui;
    QStateMachine * stateMachine;
    void setUpStateChart();
};

#endif // MAINWINDOW_H
