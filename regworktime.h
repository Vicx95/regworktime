#ifndef REGWORKTIME_H
#define REGWORKTIME_H
#include "superior.h"
#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QToolBar>
#include <QSerialPort>

#include "console.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Regworktime; }


QT_END_NAMESPACE



class Regworktime : public QWidget
{
    Q_OBJECT

public:
    Regworktime(QWidget *parent = nullptr);
    ~Regworktime();



private slots:

    void openSerialPort();

    void closeSerialPort();

    void writeData(const QByteArray &data);

    void readData();

    void handleError(QSerialPort::SerialPortError error);

    void on_LogoutButton_clicked();

    void on_backtoMenufromaddEmployee_clicked();

    void on_backToMenuFromEditEmployee_clicked();

    void on_backToMenuFromSchedule_clicked();

    void on_backToMenuFromPresence_clicked();

    void on_backToMenuFromReport_clicked();

    void on_LoginButton_clicked();

    void on_addemployee_Button_clicked();

    void on_editemployee_Button_clicked();

    void on_schedule_Button_clicked();

    void on_presence_Button_clicked();

    void on_reportButton_clicked();

    void on_CheckPresenceButton_clicked();

    void on_buttonAddEmployee_clicked();

    void on_GetEmployeeListButton_clicked();


    void on_GetEmployeeComboBox_currentIndexChanged(const QString &arg1);

    void on_buttonEditEmployee_clicked();


    void on_GetEmployeeListschedule_clicked();

    void on_scheduleGetEmployeeList_currentIndexChanged(const QString &arg1);

    void on_addSchedulebutton_clicked();

    void on_editSchedulebutton_clicked();

private:
    Ui::Regworktime *ui;
    Console* console = nullptr;
    QSerialPort* serialPortMonitor = nullptr;


};
#endif // REGWORKTIME_H
