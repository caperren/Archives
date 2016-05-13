#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>

#include "embryoprocesser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Public Variables
    Ui::MainWindow *ui;

    //Public Functions
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private:
    QDesktopWidget desktopInfo;
    EmbryoProcesser embryoProcesser;


private slots:
};

#endif // MAINWINDOW_H
