#include "dialog.h"
#include "countingthread.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    Dialog w;
    w.show();

    return a.exec();
}
