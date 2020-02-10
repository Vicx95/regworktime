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
    QString superior_id;
     QString superior_login ;

    Superior() {}
    Superior(QString login, QString password) ;
    Superior(QString id, QString name, QString surname);
    Superior(QString name, QString surname,QString phone, QString card_number, QString login, QString password, QString _repeat_password);

    void add() override;
    void edit() override;

    bool login();

    QString getLogin() { return superior_login;}
    QString getPassword() {return superior_password;}
    QString getRepeatPassword() {return repeat_password;}
    void set_ID(QString id) {this->superior_id = id ;}
    QString  get_ID() {return superior_id;}

private:


    QString superior_password;
    QString repeat_password;


};

#endif // SUPERIOR_H
