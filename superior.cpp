#include "superior.h"

Superior::Superior(QString login, QString password) :  superior_login(login), superior_password(password)
{

}


void Superior::add()
{

}

void Superior::edit()
{

}

bool Superior::login()
{
    bool success = false;
    QSqlDatabase db = QSqlDatabase::database();
    if(db.open())
    {
        QSqlQuery query(QSqlDatabase::database("Driver={MySQL ODBC 8.0 Unicode Driver};DATABASE=regworktime;"));

        query.prepare("SELECT * FROM superior WHERE superior_login = :superior_login AND superior_password = :superior_password");
        query.bindValue(":superior_login",superior_login);
        query.bindValue(":superior_password",superior_password);

        QMessageBox msgBox;

        if(!query.exec())
        {
            msgBox.setText("Błąd");
            msgBox.setInformativeText("Nie można wykonać zapytania");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            success = false;
        }
        else
        {
            while(query.next())
            {
                QString superior_loginFromDB = query.value(3).toString();
                QString superior_passwordFromDB = query.value(4).toString();
                if(superior_loginFromDB == superior_login && superior_passwordFromDB == superior_password)
                {

                   success = true ;

                }
                else
                {

                    success = false ;

                }
            }
        }
    }
    db.close();
   return  success ;
}
