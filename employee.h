#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QDate>
#include "iperson.h"

class Employee : public Person
{



public:
    Employee(QString _name, QString _surname, QString _phone_number, QDate _date);

    void add() override;
    void edit() override;


    ~Employee() override ;


    QString name ;
    QString surname;
    QString phone_number;
    QDate date_of_employment;
};

#endif // EMPLOYEE_H
