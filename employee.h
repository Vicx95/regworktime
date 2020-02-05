#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QDate>
#include "iperson.h"

class Employee : public Person
{



public:
    Employee(int id, QString _name, QString _surname, QString _phone_number, QDate _date);
    Employee(QString _name, QString _surname, QString _phone_number);

    void add() override;
    void edit() override;


    ~Employee() override ;

    int id;
    QString name ;
    QString surname;
    QString phone_number;
    QDate date_of_employment;
};

#endif // EMPLOYEE_H
