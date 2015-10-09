#include "leddisplay.h"
#include "ui_leddisplay.h"
#include <QMessageBox>
#include <QTime>

leddisplay::leddisplay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::leddisplay)
{
    ui->setupUi(this);
    foo = new varclass;
    above = new QextSerialPort();
    below  = new QextSerialPort();
    foo->setTopCOM("COM1");
    foo->setBottomCOM("COM1");
    foo->setCustomSend("                 ", 1);
    foo->setCustomSend("                 ", 2);

    connect(above, SIGNAL(readyRead()), this, SLOT(aboveReadReady()));
    connect(below, SIGNAL(readyRead()), this, SLOT(belowReadReady()));
}

leddisplay::~leddisplay()
{
    delete ui;
}

void leddisplay::on_pushButton_Connect_Wund_clicked()
{
    if(!foo->getSerialStatus(1)){
        above->setPortName(foo->getTopCOM());
        above->setBaudRate(BAUD9600);
        above->setFlowControl(FLOW_OFF);
        above->setParity(PAR_NONE);
        above->setDataBits(DATA_8);
        above->setStopBits(STOP_1);

        if(above->open(QIODevice::ReadWrite)){
            foo->toggleSerialStatus(1);
            ui->label_Top_COM_Status->setText("Connected");
            ui->label_Manual_Send_Top_Title->setText("Port: "+foo->getTopCOM());
        }else{
        QMessageBox::information(this,"Serial Debug", "Port \""+foo->getTopCOM()+"\" could not be opened.");
        }
    }else{
        QMessageBox::information(this,"Serial Debug", "The top serial port is already open.");
    }

    if(!foo->getSerialStatus(2)){
        below->setPortName(foo->getBottomCOM());
        below->setBaudRate(BAUD9600);
        below->setFlowControl(FLOW_OFF);
        below->setParity(PAR_NONE);
        below->setDataBits(DATA_8);
        below->setStopBits(STOP_1);

        if(below->open(QIODevice::ReadWrite)){
            foo->toggleSerialStatus(2);
            ui->label_Bottom_COM_Status->setText("Connected");
            ui->label_Manual_Send_Bottom_Title->setText("Port: "+foo->getBottomCOM());
        }else{
            QMessageBox::information(this,"Serial Debug", "Port \""+foo->getBottomCOM()+"\" could not be opened.");
        }
    }else{
        QMessageBox::information(this,"Serial Debug", "The bottom serial port is already open.");
    }
}

void leddisplay::on_pushButton_Disconnect_Wund_clicked()
{
    above->close();
    below->close();
    foo->toggleSerialStatus(1);
    ui->label_Top_COM_Status->setText("Disconnected");
    ui->label_Manual_Send_Top_Title->setText("Uninitialized");
    ui->textEdit_Manual_Send_Top->clear();
    foo->toggleSerialStatus(2);
    ui->label_Bottom_COM_Status->setText("Disconnected");
    ui->label_Manual_Send_Bottom_Title->setText("Uninitialized");
    ui->textEdit_Manual_Send_Bottom->clear();
}

void leddisplay::on_pushButton_Reset_Wund_clicked()
{
    above->write("RESETRESETRESETR!");
    below->write("RESETRESETRESETR!");
}

void leddisplay::on_pushButton_Send1_clicked()
{
    ui->label_String_Sent_Top->setText("HELLO WORLD!!!!!");
    ui->label_String_Sent_Bottom->setText("HOLA EL MUNDO!!!");
    above->write("HELLO WORLD!!!!!!");
    below->write("HOLA EL MUNDO!!!!");
}

void leddisplay::on_pushButton_Send2_clicked()
{
    ui->label_String_Sent_Top->setText("ISN'T ECE 152 SO");
    ui->label_String_Sent_Bottom->setText("MUCH FUN?!?!?!?!");
    above->write("ISN'T ECE 152 SO ");
    below->write("MUCH FUN?!?!?!?!?");
}

void leddisplay::on_pushButton_Send3_clicked()
{
    ui->label_String_Sent_Top->setText("DON HEER IS THE ");
    ui->label_String_Sent_Bottom->setText("BEST ECETEACHER!");
    above->write("DON HEER IS THE !");
    below->write("BEST ECETEACHER!!");
}

void leddisplay::on_pushButton_Send4_clicked()
{
    ui->label_String_Sent_Top->setText("IT'S A-ME, MARIO!");
    ui->label_String_Sent_Bottom->setText("LUIGI TOO, WAHOO!");
    above->write("IT'S A-ME, MARIO!");
    below->write("LUIGI TOO, WAHOO!");
}

void leddisplay::on_pushButton_Send5_clicked()
{
    ui->label_String_Sent_Top->setText(" OREGON  STATE  ");
    ui->label_String_Sent_Bottom->setText("  UNI    IS #1  ");
    above->write(" OREGON  STATE  !");
    below->write("  UNI    IS #1  !");
}

void leddisplay::on_pushButton_Send6_clicked()
{
    ui->label_String_Sent_Top->setText("  OUR    FIRST  ");
    ui->label_String_Sent_Bottom->setText("  YEAR  IS OVER!");
    above->write("  OUR    FIRST  !");
    below->write("  YEAR  IS OVER!!");
}

void leddisplay::on_pushButton_Send7_clicked()
{
    ui->label_String_Sent_Top->setText("MADE BY: CORWIN ");
    ui->label_String_Sent_Bottom->setText("  AND     WILL  ");
    above->write("MADE BY: CORWIN !");
    below->write("  AND     WILL  !");
}

void leddisplay::on_pushButton_Send8_clicked()
{
    QTime mytime = QTime::currentTime();
    QString send = (mytime.toString("hh:mm:ss   ap")+"   !");

    ui->label_String_Sent_Top->setText("CURRENT TIME IS:");
    ui->label_String_Sent_Bottom->setText(send.left(16).toUpper());
    above->write("CURRENT TIME IS:!");
    below->write(send.toUpper().toAscii());
}

void leddisplay::on_lineEdit_Custom_Send_Top_textChanged(const QString &arg1)
{
    foo->setCustomSend(arg1+"                 ", 1);
}

void leddisplay::on_pushButton_Custom_Send_Top_clicked()
{
    ui->label_String_Sent_Top->setText(foo->getCustomSend(1).left(16).toUpper());
    above->write((foo->getCustomSend(1).left(16)+"!").toUpper().toAscii());
}

void leddisplay::on_lineEdit_Custom_Send_Bottom_textChanged(const QString &arg1)
{
    foo->setCustomSend(arg1+"                 ", 2);
}

void leddisplay::on_pushButton_Custom_Send_Bottom_clicked()
{
    ui->label_String_Sent_Bottom->setText(foo->getCustomSend(2).left(16).toUpper());
    below->write((foo->getCustomSend(2).left(16)+"!").toUpper().toAscii());
}

void leddisplay::on_pushButton_Send_Custom_All_clicked()
{
    ui->label_String_Sent_Top->setText(foo->getCustomSend(1).left(16).toUpper());
    ui->label_String_Sent_Bottom->setText(foo->getCustomSend(2).left(16).toUpper());
    above->write((foo->getCustomSend(1).left(16)+"!").toUpper().toAscii());
    below->write((foo->getCustomSend(2).left(16)+"!").toUpper().toAscii());
}
void leddisplay::on_comboBox_Top_COM_currentIndexChanged(const QString &arg1)
{
    foo->setTopCOM(arg1);
}

void leddisplay::on_comboBox_Bottom_COM_currentIndexChanged(const QString &arg1)
{
    foo->setBottomCOM(arg1);
}

void leddisplay::aboveReadReady(){
    ui->textEdit_Manual_Send_Top->insertPlainText(above->readAll());
}

void leddisplay::belowReadReady(){
    ui->textEdit_Manual_Send_Bottom->insertPlainText(below->readAll());
}

void leddisplay::on_lineEdit_Manual_Send_Top_textChanged(const QString &arg1)
{
    foo->setManualSend(arg1, 1);
}

void leddisplay::on_lineEdit_Manual_Send_Bottom_textChanged(const QString &arg1)
{
    foo->setManualSend(arg1, 2);
}

void leddisplay::on_pushButton_Manual_Send_Top_clicked()
{
    above->write(foo->getManualSend(1).toAscii());
    ui->textEdit_Manual_Send_Top->insertPlainText(foo->getManualSend(1)+"\n");
}

void leddisplay::on_pushButton_Manual_Send_Bottom_clicked()
{
    below->write(foo->getManualSend(2).toAscii());
    ui->textEdit_Manual_Send_Bottom->insertPlainText(foo->getManualSend(2)+"\n");
}

void leddisplay::on_pushButton_Manual_Send_Clear_Screen_Top_clicked()
{
    ui->textEdit_Manual_Send_Top->clear();
}

void leddisplay::on_pushButton_Manual_Send_Clear_Screen_Bottom_clicked()
{
    ui->textEdit_Manual_Send_Bottom->clear();
}

void leddisplay::on_pushButton_Reset_Top_clicked()
{
   above->write("RESETRESETRESETR!");
   ui->textEdit_Manual_Send_Top->clear();
}

void leddisplay::on_pushButton_Reset_Bottom_clicked()
{
    below->write("RESETRESETRESETR!");
    ui->textEdit_Manual_Send_Bottom->clear();
}

void leddisplay::on_comboBox_Top_COM_editTextChanged(const QString &arg1)
{
    foo->setTopCOM(arg1);

}

void leddisplay::on_comboBox_Bottom_COM_editTextChanged(const QString &arg1)
{
    foo->setBottomCOM(arg1);
}
