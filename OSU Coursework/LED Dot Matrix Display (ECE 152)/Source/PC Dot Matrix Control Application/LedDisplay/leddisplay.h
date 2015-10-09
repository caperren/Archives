#ifndef LEDDISPLAY_H
#define LEDDISPLAY_H

#include <QMainWindow>
#include <varclass.h>
#include "qextserialport.h"

namespace Ui {
class leddisplay;
}

class leddisplay : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit leddisplay(QWidget *parent = 0);
    ~leddisplay();
    
private slots:

    void on_pushButton_Connect_Wund_clicked();

    void on_pushButton_Disconnect_Wund_clicked();

    void on_pushButton_Reset_Wund_clicked();

    void on_pushButton_Send1_clicked();

    void on_pushButton_Send2_clicked();

    void on_pushButton_Send3_clicked();

    void on_pushButton_Send4_clicked();

    void on_pushButton_Send5_clicked();

    void on_pushButton_Send6_clicked();

    void on_pushButton_Send7_clicked();

    void on_pushButton_Send8_clicked();

    void on_lineEdit_Custom_Send_Top_textChanged(const QString &arg1);

    void on_pushButton_Custom_Send_Top_clicked();

    void on_lineEdit_Custom_Send_Bottom_textChanged(const QString &arg1);

    void on_pushButton_Custom_Send_Bottom_clicked();

    void on_pushButton_Send_Custom_All_clicked();

    void on_comboBox_Top_COM_currentIndexChanged(const QString &arg1);

    void on_comboBox_Bottom_COM_currentIndexChanged(const QString &arg1);

    void aboveReadReady();

    void belowReadReady();

    void on_lineEdit_Manual_Send_Top_textChanged(const QString &arg1);

    void on_lineEdit_Manual_Send_Bottom_textChanged(const QString &arg1);

    void on_pushButton_Manual_Send_Top_clicked();

    void on_pushButton_Manual_Send_Bottom_clicked();

    void on_pushButton_Manual_Send_Clear_Screen_Top_clicked();

    void on_pushButton_Manual_Send_Clear_Screen_Bottom_clicked();

    void on_pushButton_Reset_Top_clicked();

    void on_pushButton_Reset_Bottom_clicked();

    void on_comboBox_Top_COM_editTextChanged(const QString &arg1);

    void on_comboBox_Bottom_COM_editTextChanged(const QString &arg1);

private:
    Ui::leddisplay *ui;
    varclass *foo;
    QextSerialPort *above;
    QextSerialPort *below;
};

#endif // LEDDISPLAY_H
