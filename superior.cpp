#include "superior.h"

Superior::Superior(QString login, QString password) :  superior_login(login), superior_password(password)
{

}


Superior::Superior(QString name, QString surname,QString phone, QString card_number, QString login, QString password, QString _repeat_password)
    :   superior_name(name), superior_surname(surname),phone_number(phone), card_number(card_number), superior_login(login), superior_password(password), repeat_password(_repeat_password)
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
        QSqlQuery query;
      QByteArray hashPassword =(superior_password.toStdString().c_str());
      hashPassword = QCryptographicHash::hash(hashPassword,QCryptographicHash::Sha512).toHex();
      superior_password = QString::fromStdString(hashPassword.toStdString());

        query.prepare("SELECT * FROM employee WHERE superior_login = '"+superior_login+"' AND superior_password = '"+superior_password+"'");

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
                QString superior_loginFromDB = query.value(6).toString();
                QString superior_passwordFromDB = query.value(7).toString();

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
