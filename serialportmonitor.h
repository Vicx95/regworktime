#ifndef SERIALPORTMONITOR_H
#define SERIALPORTMONITOR_H
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <memory>
#include <QMessageBox>
#include <QDebug>
class SerialPortMonitor
{

public:
    explicit SerialPortMonitor();


    void openSerialPort();
    void closeSerialPort();
    void readData();


private:
    std::unique_ptr<QSerialPort> serialPort ;

};

#endif // SERIALPORTMONITOR_H
