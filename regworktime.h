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
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>
#include <QPrintDialog>
#include <QTextTable>
#include <QTableWidget>
#include "editregisterevent.h"

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


    void on_radioButtonEmployee_clicked();

    void on_radioButtonSuperior_clicked();

    void on_backtoMenu_clicked();

    void on_manualRegisterButton_clicked();

    void on_addManualRegister_clicked();

    void on_GetEmployeeListManualRegister_clicked();

    void on_manualRegisterGetEmployeeList_currentIndexChanged(const QString &arg1);

    void on_getEmployeeListGenerateSchedule_clicked();

    void on_generateScheduleEmployeeList_currentIndexChanged(const QString &arg1);

    void on_generateButton_clicked();

private:
    void registry(const QString& employeeID);
    Ui::Regworktime *ui;
    Console* console = nullptr;
    QSerialPort* serialPortMonitor = nullptr;


};
#endif // REGWORKTIME_H
