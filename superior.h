#ifndef SUPERIOR_H
#define SUPERIOR_H
#include "iperson.h"
#include <QSqlDatabase>
#include "regworktime.h"
#include <QMessageBox>
class Superior : public Person
{
public:
    QString superior_name;
    QString superior_surname;
    QString phone_number;
    QString card_number;

    Superior(QString login, QString password) ;
    Superior(QString name, QString surname,QString phone, QString card_number, QString login, QString password, QString _repeat_password);

    void add() override;
    void edit() override;

    bool login();

    QString getLogin() { return superior_login;}
    QString getPassword() {return superior_password;}
    QString getRepeatPassword() {return repeat_password;}
    void set_ID(int id) {this->superior_id = id ;}
    int  get_ID() {return superior_id;}

private:
    int superior_id;

    QString superior_login ;
    QString superior_password;
    QString repeat_password;


};

#endif // SUPERIOR_H
