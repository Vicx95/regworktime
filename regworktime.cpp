#include "regworktime.h"
#include "ui_regworktime.h"
#include <QMessageBox>
#include "database.h"
Regworktime::Regworktime(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Regworktime)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    Database db ;

    if(db.get_database().open())
    {
        ui->databaseStatus->setText("Nawiązano połączenie z bazą danych...");
    }
    else
    {
        ui->databaseStatus->setText("Brak połączenia z bazą danych...");
    }
}

Regworktime::~Regworktime()
{
    delete ui;
}


void Regworktime::on_LoginButton_clicked()
{
    QString superior_login = ui->UserloginField->text() ;
    QString superior_password = ui->PasswordField->text();
    Database database;
    if(database.get_database().open())
    {
        QSqlQuery query(QSqlDatabase::database("Driver={MySQL ODBC 8.0 Unicode Driver};DATABASE=regworktime;"));
        query.prepare("SELECT * FROM superior WHERE superior_login = :superior_login AND superior_password = :superior_password");
        query.bindValue(":superior_login",superior_login);
        query.bindValue(":superior_password",superior_password);
        if(!query.exec())
        {
            QMessageBox::information(this,"Błąd","Nie można wykonać zapytania");
        }
        else
        {
            while(query.next())
            {
                QString superior_loginFromDB = query.value(4).toString();
                QString superior_passwordFromDB = query.value(5).toString();
                if(superior_loginFromDB == superior_login && superior_passwordFromDB == superior_password)
                {
                    QMessageBox::information(this,"Sukces","Logowanie powiodło się");
                    ui->stackedWidget->setCurrentIndex(1);

                }
                else
                {
                    QMessageBox::information(this,"Błąd","Nieprawidłowy login lub hasło");

                }
            }
        }
    }
}


void Regworktime::on_LogoutButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Regworktime::on_backtoMenufromaddEmployee_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Regworktime::on_backToMenuFromEditEmployee_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void Regworktime::on_backToMenuFromSchedule_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Regworktime::on_backToMenuFromPresence_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Regworktime::on_backToMenuFromReport_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}




void Regworktime::on_addemployee_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Regworktime::on_editemployee_Button_clicked()
{
     ui->stackedWidget->setCurrentIndex(3);
}

void Regworktime::on_schedule_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void Regworktime::on_presence_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void Regworktime::on_reportButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void Regworktime::on_CheckPresenceButton_clicked()
{

    Database database;
    bool presence = 1 ;
    if(database.get_database().open())
    {
        QSqlQueryModel *model = new QSqlQueryModel();

        QSqlQuery query(QSqlDatabase::database("Driver={MySQL ODBC 8.0 Unicode Driver};DATABASE=regworktime;"));

        query.prepare("SELECT * FROM employee  WHERE is_present = 1 ");
        query.bindValue(":is_present",presence);
        if(!query.exec())
        {
            QMessageBox::information(this,"Błąd","Nie można wykonać zapytania");
        }
        else
        {
            model->setQuery(query);
            ui->PresenceView->setModel(model);

        }

     }
  //  database.close_db();
}



