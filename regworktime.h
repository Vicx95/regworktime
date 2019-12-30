#ifndef REGWORKTIME_H
#define REGWORKTIME_H

#include <QWidget>

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

private:
    Ui::Regworktime *ui;
};
#endif // REGWORKTIME_H
