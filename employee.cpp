#include "employee.h"

Employee::Employee(int id, QString _name, QString _surname, QString _phone_number, int _card_number) :
                    id(id),name(_name), surname(_surname), phone_number(_phone_number), card_number(_card_number)
{

}


Employee::Employee(QString _name, QString _surname, QString _phone_number,int _card_number):
                    name(_name), surname(_surname), phone_number(_phone_number), card_number(_card_number)
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
