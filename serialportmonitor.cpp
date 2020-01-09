#include "serialportmonitor.h"

SerialPortMonitor::SerialPortMonitor(QObject *parent) : QObject(parent)
{

}


void SerialPortMonitor::openSerialPort()
{
    serialPort->setPortName("COM3");
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if(serialPort->open(QIODevice::ReadWrite))
        return;
    else
    {
      qDebug() << "Nie można otworzyć portu szeregowego\n";
    }
}


void SerialPortMonitor::closeSerialPort()
{
    if(serialPort->isOpen())
    {
        serialPort->close();
    }
}


void SerialPortMonitor::readData()
{
    const QByteArray data = serialPort->readAll();


    int idFromCard = data.toInt();
    QString time_format = "HH:mm:ss";
    QString date_format = "yyyy-MM-dd";
}
