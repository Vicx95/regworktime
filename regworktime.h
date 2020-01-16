#ifndef REGWORKTIME_H
#define REGWORKTIME_H
#include "serialportmonitor.h"
#include "superior.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QWidget>
#include <QDebug>
#include <QObject>
QT_BEGIN_NAMESPACE
namespace Ui { class Regworktime; }
QT_END_NAMESPACE




class Regworktime : public QWidget
{
    Q_OBJECT

public:
    SerialPortMonitor serialPortMonitor;

    Regworktime(QWidget *parent = nullptr);
    ~Regworktime();

    SerialPortMonitor getSerialPort();


private slots:
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

private:
    Ui::Regworktime *ui;


};
#endif // REGWORKTIME_H
