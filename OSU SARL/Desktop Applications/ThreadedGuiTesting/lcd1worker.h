#ifndef LCD1WORKER_H
#define LCD1WORKER_H

#include <QObject>

class LCD1Worker : public QObject
{
    Q_OBJECT
public:
    explicit LCD1Worker(QObject *parent = 0);

signals:

public slots:
    void IncrementLCD();

};

#endif // LCD1WORKER_H
