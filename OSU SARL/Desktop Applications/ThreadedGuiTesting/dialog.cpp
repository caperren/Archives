#include "ui_dialog.h"
#include "dialog.h"
#include "countingthread.h"
#include <QtCore>
#include <QThread>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    counter1thread = NULL;
    counter2thread = NULL;
    counter3thread = NULL;
    counter4thread = NULL;

    LCDThread1 = NULL;
    LCDThread2 = NULL;
    LCDThread3 = NULL;
    LCDThread4 = NULL;
//    counter1thread = new QThread(this);
//    counter2thread = new QThread(this);
//    counter3thread = new QThread(this);
//    counter4thread = new QThread(this);

//    LCDThread1 = new CountingThread;
//    LCDThread2 = new CountingThread;
//    LCDThread3 = new CountingThread;
//    LCDThread4 = new CountingThread;

//    connect(LCDThread1, SIGNAL(NumberChanged(int)), this, SLOT(onLCD1NumberChanged(int)));
//    connect(LCDThread2, SIGNAL(NumberChanged(int)), this, SLOT(onLCD2NumberChanged(int)));
//    connect(LCDThread3, SIGNAL(NumberChanged(int)), this, SLOT(onLCD3NumberChanged(int)));
//    connect(LCDThread4, SIGNAL(NumberChanged(int)), this, SLOT(onLCD4NumberChanged(int)));
    DoSetup();

    connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));

    timer->start(300);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::DoSetup()
{
    if(LCDThread1 != NULL){
        //LCDThread1->
        delete(LCDThread1);
    }

    if(LCDThread2 != NULL){
        delete(LCDThread2);
    }

    if(LCDThread3 != NULL){
        delete(LCDThread3);
    }

    if(LCDThread4 != NULL){
        delete(LCDThread4);
    }

    counter1thread = new QThread(this);
    counter2thread = new QThread(this);
    counter3thread = new QThread(this);
    counter4thread = new QThread(this);

    LCDThread1 = new CountingThread;
    LCDThread2 = new CountingThread;
    LCDThread3 = new CountingThread;
    LCDThread4 = new CountingThread;

    connect(LCDThread1, SIGNAL(NumberChanged(int)), this, SLOT(onLCD1NumberChanged(int)));
    connect(LCDThread2, SIGNAL(NumberChanged(int)), this, SLOT(onLCD2NumberChanged(int)));
    connect(LCDThread3, SIGNAL(NumberChanged(int)), this, SLOT(onLCD3NumberChanged(int)));
    connect(LCDThread4, SIGNAL(NumberChanged(int)), this, SLOT(onLCD4NumberChanged(int)));
}

void Dialog::on_startButton1_clicked()
{
    LCDThread1->StartThread();
    LCDThread1->ClrPause();
}

void Dialog::on_startButton2_clicked()
{
    LCDThread2->StartThread();
    LCDThread2->ClrPause();
}

void Dialog::on_startButton3_clicked()
{
    LCDThread3->StartThread();
    LCDThread3->ClrPause();
}

void Dialog::on_startButton4_clicked()
{
    LCDThread4->StartThread();
    LCDThread4->ClrPause();
}

void Dialog::onLCD1NumberChanged(int newVal)
{
    ui->lcdNumber1->display(newVal);
}

void Dialog::onLCD2NumberChanged(int newVal)
{
    ui->lcdNumber2->display(newVal);
}

void Dialog::onLCD3NumberChanged(int newVal)
{
    ui->lcdNumber3->display(newVal);
}

void Dialog::onLCD4NumberChanged(int newVal)
{
    ui->lcdNumber4->display(newVal);
}

void Dialog::on_pauseButton1_clicked()
{
    LCDThread1->SetPause();
}

void Dialog::on_pauseButton2_clicked()
{
    LCDThread2->SetPause();
}

void Dialog::on_pauseButton3_clicked()
{
    LCDThread3->SetPause();
}

void Dialog::on_pauseButton4_clicked()
{
    LCDThread4->SetPause();
}

void Dialog::updateProgressBar()
{
    int progress = ((long(LCDThread1->PercentComplete())+LCDThread2->PercentComplete()+LCDThread3->PercentComplete()+LCDThread4->PercentComplete())/4);
    if(progress == 99){
        progress = 100;
    }
    ui->progressBar->setValue(progress);
}

void Dialog::on_pushButton_clicked()
{
    DoSetup();
}

void Dialog::on_pushButton_2_clicked()
{

}
