#ifndef SERIALPORTMONITOR_H
#define SERIALPORTMONITOR_H
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <memory>
#include <QMessageBox>
#include <QDebug>
class SerialPortMonitor : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortMonitor(QObject *parent = nullptr);


    void openSerialPort();
    void closeSerialPort();
    void readData();

signals:



public slots:


private:
    std::unique_ptr<QSerialPort> serialPort ;

};

#endif // SERIALPORTMONITOR_H
