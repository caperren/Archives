#include "varclass.h"

varclass::varclass()
{
    serial1open = false;
    serial2open = false;
}

void varclass::setTopCOM(QString set){
    topCOM = set;
}

void varclass::setBottomCOM(QString set){
    bottomCOM = set;
}

void varclass::setCustomSend(QString set, int whichvar){
    switch(whichvar){
       case(1):
           customTopSend = set;
           break;
       case(2):
           customBottomSend = set;
           break;
    }
}

void varclass::setManualSend(QString set, int whichport){
    switch(whichport){
       case(1):
           manualSendTop = set;
           break;
       case(2):
           manualSendBottom = set;
           break;
    }
}

QString varclass::getTopCOM(){
    return topCOM;
}

QString varclass::getBottomCOM(){
    return bottomCOM;
}

QString varclass::getCustomSend(int whichvar){
    switch(whichvar){
       case(1):
           return customTopSend;
           break;
       case(2):
           return customBottomSend;
           break;
    }
    return "Broken";
}

QString varclass::getManualSend(int whichport){
    switch(whichport){
       case(1):
           return manualSendTop;
           break;
       case(2):
           return manualSendBottom;
           break;
    }
    return "Error";
}

 int varclass::getSerialStatus(int whichport){
     switch(whichport){
        case(1):
            return serial1open;
            break;
        case(2):
            return serial2open;
            break;
     }

     return -1;
 }

 void varclass::toggleSerialStatus(int whichport){
     switch(whichport){
        case(1):
            serial1open = !serial1open;
            break;
        case(2):
            serial2open = !serial2open;
            break;
     }
 }


