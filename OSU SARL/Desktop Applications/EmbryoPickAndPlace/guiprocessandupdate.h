#ifndef GUIPROCESSANDUPDATE_H
#define GUIPROCESSANDUPDATE_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QDesktopWidget>

#include "mainwindow.h"

class GuiProcessAndUpdate : public QMainWindow
{
    Q_OBJECT
public:
    explicit GuiProcessAndUpdate(QWidget *parent = 0, QMainWindow *MainWindowInput = 0);

signals:

public slots:

private:
    QThread *GuiProcessAndUpdateThread;
    QTimer *mainGuiTimer;



};

#endif // GUIPROCESSANDUPDATE_H
