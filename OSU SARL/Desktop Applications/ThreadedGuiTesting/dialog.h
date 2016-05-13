#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "dialog.h"
#include "countingthread.h"
#include <QTimer>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void DoSetup();



private slots:
    void on_startButton1_clicked();

    void on_startButton2_clicked();

    void on_startButton3_clicked();

    void on_startButton4_clicked();

    void onLCD1NumberChanged(int);

    void onLCD2NumberChanged(int);

    void onLCD3NumberChanged(int);

    void onLCD4NumberChanged(int);

    void on_pauseButton1_clicked();

    void on_pauseButton2_clicked();

    void on_pauseButton3_clicked();

    void on_pauseButton4_clicked();

    void updateProgressBar();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:


    Ui::Dialog *ui;
    QThread *counter1thread, *counter2thread, *counter3thread, *counter4thread;
    CountingThread *LCDThread1, *LCDThread2, *LCDThread3, *LCDThread4;
    QTimer *timer;
};

#endif // DIALOG_H
