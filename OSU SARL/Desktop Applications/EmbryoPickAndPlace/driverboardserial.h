#ifndef DRIVERBOARDSERIAL_H
#define DRIVERBOARDSERIAL_H

#include "qextserialport.h"

class DriverBoardSerial
{
public:
    DriverBoardSerial();
    void sendDriverControlPacket();
    int *getDriverResponsePacket();


private:
    QString requestedPort;
    int requestedBaud;

    QextSerialPort *driverBoardSerial;
};

#endif // DRIVERBOARDSERIAL_H
