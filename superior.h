#ifndef SUPERIOR_H
#define SUPERIOR_H
#include "iperson.h"
#include "database.h"
#include "regworktime.h"
#include <QMessageBox>
class Superior : public Person
{
public:
    Superior(QString login, QString password) ;


    void add() override;
    void edit() override;

    bool login();

    QString getLogin() { return superior_login;}
    QString getPassword() {return superior_password;}

private:
    QString superior_login ;
    QString superior_password;
};

#endif // SUPERIOR_H
