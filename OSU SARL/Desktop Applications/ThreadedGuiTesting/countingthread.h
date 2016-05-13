#ifndef COUNTINGTHREAD_H
#define COUNTINGTHREAD_H

#include <QObject>
#include <QThread>

class CountingThread : public QObject
{
    Q_OBJECT
public:
    explicit CountingThread(QObject *parent = 0);
    void SetupCounterThread(QThread *cThread);
    void run();
    void SetPause();
    void ClrPause();
    int PercentComplete();
    void StartThread();

signals:
    void NumberChanged(int);

public slots:
    void onStart();

private:
    QThread *thread;
    bool Paused;
    long CurrentCount;
};

#endif // COUNTINGTHREAD_H
