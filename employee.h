#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QDate>
#include "iperson.h"

class Employee : public Person
{
    Q_OBJECT
public:
    explicit Employee(QObject *parent = nullptr);

signals:

public:
    Employee();

    void add() override;
    void edit() override;


    ~Employee() override ;
public slots:

private:
    QString name ;
    QString surname;
    QString department;
    QString phone_number;
    QDate date_of_employment;
};

#endif // EMPLOYEE_H
