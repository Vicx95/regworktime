#include "editregisterevent.h"

EditRegisterEvent::EditRegisterEvent()
{

}


EditRegisterEvent::~EditRegisterEvent()
{

}


void EditRegisterEvent::saveEvent(const QString &superiorID, const QString &description)
{

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery registerQuery;
    registerQuery.prepare("INSERT INTO event_registry (employee_id,event_description) VALUES"
                          " ( '"+superiorID+"' , '"+description+"')  ");
    registerQuery.exec();
}
