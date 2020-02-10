#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QDate>
#include "iperson.h"

class Employee : public Person
{



public:
    Employee(QString id, QString _name, QString _surname, QString _phone_number, QString _card_number);
    Employee(QString _name, QString _surname, QString _phone_number,QString _card_number);

    void add() override;
    void edit() override;


    ~Employee() override ;

    QString id;
    QString name ;
    QString surname;
    QString phone_number;
    QString card_number;
};

#endif // EMPLOYEE_H
