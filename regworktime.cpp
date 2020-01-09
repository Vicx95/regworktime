#include "regworktime.h"
#include "ui_regworktime.h"
#include <QDate>
#include <QMessageBox>
#include "employee.h"
#include "database.h"
#include <QFileInfo>
Regworktime::Regworktime(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Regworktime)
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
    db.close_db();
}

Regworktime::~Regworktime()
{
    delete ui;
   // serialPortMonitor.closeSerialPort();
}


void Regworktime::on_LoginButton_clicked()
{
    QString superior_login = ui->UserloginField->text() ;
    QString superior_password = ui->PasswordField->text();
    Superior superior(superior_login,superior_password);

    if(superior_login.isEmpty() || superior_password.isEmpty())
        QMessageBox::information(this,"Błąd","Pola nie mogą być puste");

     else if(superior.login() == true)
     {
         ui->stackedWidget->setCurrentIndex(1);
     }
     else
     {
        ui->stackedWidget->setCurrentIndex(0);
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

        delete model;

     }
    database.close_db();
}




void Regworktime::on_buttonAddEmployee_clicked()
{
    Database database;
    Employee employee(ui->fieldName->text(),
                      ui->fieldSurname->text(),
                      ui->fieldPhone->text(),
                      ui->fieldDate->date());

    QString dateFormat = "yyyy-MM-dd";
    QString dateString = employee.date_of_employment.toString(dateFormat);

    if(database.get_database().open())
    {

        QSqlQuery query(QSqlDatabase::database("Driver={MySQL ODBC 8.0 Unicode Driver};DATABASE=regworktime;"));
       // query.prepare("INSERT INTO employee (employee_name, employee_surname,phone_number,date_of_employment) "
         //             "values(?,?,?,?)");

        query.prepare("INSERT INTO employee SET employee_name = '"+employee.name+"', employee_surname = '"+employee.surname+"', phone_number ='"+employee.phone_number+"',"
                      "date_of_employment = '"+dateString+"' ");

        if(query.exec())
        {
            QMessageBox::information(this,"Sukces","Dodano pracownika do bazy danych");
            ui->fieldName->clear();
            ui->fieldSurname->clear();
            ui->fieldPhone->clear();
            ui->fieldDate->clear();
            database.close_db();
        }
        else
        {
           QMessageBox::critical(this,tr("Error::"),query.lastError().text());
             database.close_db();
        }

     }

}

void Regworktime::on_GetEmployeeListButton_clicked()
{
    Database database ;

    QSqlQueryModel* model = new QSqlQueryModel();

    if(database.get_database().open())
    {
       model->setQuery("SELECT employee_id, employee_name FROM employee");

       model->setHeaderData(0,Qt::Horizontal,tr("ID"));
       model->setHeaderData(1,Qt::Horizontal,tr("Imię"));

       QTableView * view = new QTableView;
       view->verticalHeader()->hide();
       view->horizontalHeader()->hide();
       view->setSelectionBehavior(QAbstractItemView::SelectRows);
       view->setSelectionMode(QAbstractItemView::SingleSelection);
       ui->GetEmployeeComboBox->setModel(model);
       ui->GetEmployeeComboBox->setView(view);

       delete view ;

    }

     delete model;
    database.close_db();
}


void Regworktime::on_GetEmployeeComboBox_currentIndexChanged(int index)
{
   Database database;


    QString id = QString::number(index = ui->GetEmployeeComboBox->currentIndex());

    QSqlQuery query ;
   query.prepare("SELECT * FROM employee WHERE employee_id = '"+id+"'");

    if(database.get_database().open())
    {
        if(query.exec())
        {
            while(query.next())
            {
                ui->fieldeditName->setText(query.value(1).toString());
                ui->fieldeditSurname->setText(query.value(2).toString());
                ui->fieldeditPhone->setText(query.value(3).toString());
                ui->fieldeditDate->setDate(query.value(4).toDate());
            }
            database.close_db();
        }
        else
        {
            QMessageBox::critical(this,"Błąd",query.lastError().text());
        }
    }
}
