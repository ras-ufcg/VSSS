#ifndef DIALOG_H
#define DIALOG_H


#include <QDialog>
#include <QtSerialPort/QSerialPort>
//#include "mainwindow.h"
//#include "mainwindow.cpp"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void setCoordenadas(int x_, int y_);
    void updateLED(QString);
    //int getCoordenadaX();
    //int getCoordenadaY();
    ~Dialog();


private slots:
    void on_checkBox_stateChanged(int arg1);


private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 6790;
    static const quint16 arduino_uno_product_id = 29987;
    QString arduino_port_name;
    bool arduino_is_available;
    //class MainWindow;
    int x = 0;
    int y = 0;

};

#endif // DIALOG_H
