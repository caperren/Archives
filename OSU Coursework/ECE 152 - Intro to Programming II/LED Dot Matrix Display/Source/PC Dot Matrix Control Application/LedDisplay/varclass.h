#ifndef VARCLASS_H
#define VARCLASS_H

#include <QtCore>
class varclass
{
public:
    varclass();
    void setTopCOM(QString);
    void setBottomCOM(QString);
    void setCustomSend(QString, int);
    void setManualSend(QString, int);
    void toggleSerialStatus(int);
    QString getTopCOM();
    QString getBottomCOM();
    QString getCustomSend(int);
    QString getManualSend(int);
    int getSerialStatus(int);

private:
    QString topCOM;
    QString bottomCOM;
    QString customTopSend;
    QString customBottomSend;
    QString manualSendTop;
    QString manualSendBottom;
    bool serial1open;
    bool serial2open;
};

#endif // VARCLASS_H
