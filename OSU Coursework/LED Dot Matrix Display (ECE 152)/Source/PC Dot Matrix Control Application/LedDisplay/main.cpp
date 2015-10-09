#include <QtGui/QApplication>
#include <QtCore>
#include <string>
#include "qextserialport.h"
#include "varclass.h"
#include "leddisplay.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    leddisplay w;
    w.show();
    
    return a.exec();
}
