#include "employee.h"

Employee::Employee(int id, QString _name, QString _surname, QString _phone_number, QDate _date) :
                    id(id),name(_name), surname(_surname), phone_number(_phone_number), date_of_employment(_date)
{

}


Employee::Employee(QString _name, QString _surname, QString _phone_number):
                    name(_name), surname(_surname), phone_number(_phone_number)
{

}

void Employee::add()
{



}

void Employee::edit()
{

}


Employee::~Employee()
{

}
