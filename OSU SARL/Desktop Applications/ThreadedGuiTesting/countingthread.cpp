#include "countingthread.h"
#include <QThread>
#include <QMutex>
#include <QDebug>

CountingThread::CountingThread(QObject *parent) : QObject(parent){
    Paused = false;
    CurrentCount = 0;
    thread = new QThread;

    this->SetupCounterThread(thread);
    this->moveToThread(thread);
}

void CountingThread::SetupCounterThread(QThread *cThread)
{
    connect(cThread, SIGNAL(started()), this, SLOT(onStart()));
}

void CountingThread::run()
{

    for(long i = 0 ; i < 10000 ; i++){
        while(Paused){
            thread->msleep(100);
        }
        CurrentCount = i;
       // qDebug() << "CurrentCount = " << CurrentCount;
        emit NumberChanged(i);
        thread->msleep(10);
    }
}

void CountingThread::SetPause()
{
    Paused = true;
}

void CountingThread::ClrPause()
{
    Paused = false;
}

int CountingThread::PercentComplete()
{

    return CurrentCount/100;
}

void CountingThread::StartThread()
{
    thread->start();
}

void CountingThread::onStart()
{
    run();
}

