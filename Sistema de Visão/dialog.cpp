#include "dialog.h"
#include "ui_dialog.h"
//#include "mainwindow.h"
//#include "mainwindow.cpp"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    arduino_is_available = false;
    arduino_port_name = "";
    arduino = new QSerialPort;

    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
            if(serialPortInfo.hasVendorIdentifier()){
                qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
            }
            qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
            if(serialPortInfo.hasProductIdentifier()){
                qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
            }
        }

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
           if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
               if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                   if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                       arduino_port_name = serialPortInfo.portName();
                       arduino_is_available = true;
                       //qDebug() << "cheguei aqui";
                   }
               }
           }
       }

    if(arduino_is_available){
           // open and configure the serialport
           arduino->setPortName(arduino_port_name);
           arduino->open(QSerialPort::WriteOnly);
           arduino->setBaudRate(QSerialPort::Baud9600);
           arduino->setDataBits(QSerialPort::Data8);
           arduino->setParity(QSerialPort::NoParity);
           arduino->setStopBits(QSerialPort::OneStop);
           arduino->setFlowControl(QSerialPort::NoFlowControl);
           //qDebug() << "cheguei aqui também";
       }else{
           // give error message if not available
           QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
       }
   }

//}


Dialog::~Dialog()
{
    if(arduino->isOpen()){
            arduino->close();
        }
    delete ui;
}

void Dialog::setCoordenadas (int x_, int y_) {
    x = x_;
    y = y_;

    //qDebug() << x << "," << y;
}

/*int Dialog::getCoordenadaX(){
    return x;
}

int Dialog::getCoordenadaY(){
    return y;
}*/

void Dialog::on_checkBox_stateChanged(int arg1)
{
   //int a -> getCoordenadaX();
   //int b -> getCoordenadaY();
    /*if (arg1 == 2) {
        Dialog::updateLED(QString("L%1"));
        //qDebug() << arg1;
        //qDebug() << a << "," << b;
    } else {
        Dialog::updateLED(QString("D%1"));
        //qDebug() << arg1;
    }*/
}

void Dialog::updateLED(QString command)
{
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}
