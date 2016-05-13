#include <QtCore>
#include <QApplication>
#include <QRect>
#include <QDebug>
#include <QTableView>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "guiprocessandupdate.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //Class Initializations


    //Signal and Slot Connections
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));  //Connect exit button to application close
    qDebug() << "Screen Width = " << QApplication::desktop()->geometry().width();
    ui->tabWidget->setFixedWidth(QApplication::desktop()->geometry().width()-10);    //Set Width of tabWidget to ten pixels less than the screen resolution width
    ui->tabWidget->setFixedHeight((QApplication::desktop()->geometry().height()*4)/5); //Set height of tabWidget to 9/10 the size of the screen height
    ui->tabWidget->setTabText(1, "Table Data");
    for(int i = 0 ; i < 100 ; i++){
        ui->textGeneralStatusAndNotifications->append("This is a test...");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
