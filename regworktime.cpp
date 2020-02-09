#include "regworktime.h"
#include "ui_regworktime.h"
#include <QAction>

#include "employee.h"

#include <QFileInfo>
Regworktime::Regworktime(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Regworktime),
      console(new Console),
      serialPortMonitor(new QSerialPort(this))


{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);


    auto toolbar = new QToolBar();
    toolbar->addAction("Połącz",this,SLOT(openSerialPort()));
    toolbar->setStyleSheet("QToolBar {border:10 px solid blue}");
    auto layout = new QVBoxLayout();
    layout->setMenuBar(toolbar);
    layout->addWidget(console);
    layout->setContentsMargins(0,0,0,0);
    auto dockContent = new QWidget();
    dockContent->setLayout(layout);
    ui->dockWidget->setWidget(dockContent);
    ui->dockWidget->setFeatures(ui->dockWidget->features()& QDockWidget::NoDockWidgetFeatures);




    connect(serialPortMonitor,&QSerialPort::errorOccurred,this,&Regworktime::handleError);
    connect(serialPortMonitor,&QSerialPort::readyRead,this,&Regworktime::readData);
    connect(console,&Console::getData,this,&Regworktime::writeData);

    QSqlDatabase db = QSqlDatabase::database();

    if(db.open())
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


void Regworktime::openSerialPort()
{
    serialPortMonitor->setPortName("COM3");
    serialPortMonitor->setBaudRate(QSerialPort::Baud9600);
    serialPortMonitor->setDataBits(QSerialPort::Data8);
    serialPortMonitor->setParity(QSerialPort::NoParity);
    serialPortMonitor->setStopBits(QSerialPort::OneStop);
    serialPortMonitor->setFlowControl(QSerialPort::NoFlowControl);

    if(serialPortMonitor->open(QIODevice::ReadWrite))
    {
        console->setEnabled(true);
        console->setLocalEchoEnabled(true);
    }
    else
    {
        QMessageBox::critical(this,tr("Błąd"),serialPortMonitor->errorString());
    }
}


void Regworktime::closeSerialPort()
{
    if(serialPortMonitor->isOpen())
    {
        serialPortMonitor->close();
        console->setEnabled(false);

    }
}


void Regworktime::writeData(const QByteArray& data)
{
    serialPortMonitor->write(data);
}

void Regworktime::readData()
{
    const QByteArray data = serialPortMonitor->readAll();
    console->putData(data);
}


void Regworktime::handleError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(this, tr("Critical Error"), serialPortMonitor->errorString());
        closeSerialPort();
    }
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

    QSqlDatabase db = QSqlDatabase::database();

    bool presence = 1 ;
    if(db.open())
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

}

void Regworktime::on_buttonAddEmployee_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if(ui->radioButtonEmployee->isChecked())
    {
        Employee employee(ui->fieldName->text(),
                          ui->fieldSurname->text(),
                          ui->fieldPhone->text(),
                          ui->fieldCardID->text()
                          );
        if(employee.name.isEmpty() && employee.surname.isEmpty()&& employee.phone_number.isEmpty()&& employee.card_number.isEmpty() )
            QMessageBox::critical(this,"Błąd","Wszystkie pola muszą być wypełnione");

        if(db.open())
        {

            QSqlQuery addEmployeeQuery;
            addEmployeeQuery.prepare("INSERT INTO employee SET employee_name = '"+employee.name+"', employee_surname = '"+employee.surname+"', employee_phone ='"+employee.phone_number+"',"
                          "employee_card_id = '"+employee.card_number+"' ");
            if(addEmployeeQuery.exec())
            {
                QMessageBox::information(this,"Sukces","Dodano pracownika do bazy danych");
                ui->fieldName->clear();
                ui->fieldSurname->clear();
                ui->fieldPhone->clear();
                ui->fieldCardID->clear();
                db.close();
            }
            else
            {
                QMessageBox::critical(this,tr("Błąd"),"Istnieje już taki pracownik");
                 db.close();
            }

         }
    }

    else if(ui->radioButtonSuperior->isChecked())
    {
        QString makeLogin = ui->fieldName->text()[0]+ ui->fieldSurname->text();
        ui->fieldLogin->setText(makeLogin);


        Superior superior(ui->fieldName->text(),
                          ui->fieldSurname->text(),
                          ui->fieldPhone->text(),
                          ui->fieldCardID->text(),
                          ui->fieldLogin->text(),
                          ui->fieldPassword->text(),
                          ui->fieldRepeatPassword->text());
        if(superior.superior_name.isEmpty() && superior.superior_surname.isEmpty()&&
           superior.phone_number.isEmpty() && superior.card_number.isEmpty()&&
           superior.getPassword().isEmpty() && superior.getRepeatPassword().isEmpty()
           )
            QMessageBox::critical(this,"Błąd","Wszystkie pola muszą być wypełnione");

        if(ui->fieldPassword->text() == ui->fieldRepeatPassword->text())
        {

            QByteArray hashPassword =(superior.getPassword().toStdString().c_str());

            hashPassword = QCryptographicHash::hash(hashPassword,QCryptographicHash::Sha512).toHex();
            QString password = QString::fromStdString(hashPassword.toStdString());
            QSqlQuery addSuperiorQuery;
            addSuperiorQuery.prepare("INSERT INTO employee SET employee_name='"+superior.superior_name+"', employee_surname='"+superior.superior_surname+"',"
                                "employee_phone ='"+superior.phone_number+"', employee_card_id = '"+superior.card_number+"', superior_login= '"+superior.getLogin()+"', superior_password='"+password+"' ");
                if(addSuperiorQuery.exec())
                {
                    QMessageBox::information(this,"Sukces","Dodano przełożonego do bazy danych");
                    ui->fieldName->clear();
                    ui->fieldSurname->clear();
                    ui->fieldPhone->clear();
                    ui->fieldCardID->clear();
                    ui->fieldLogin->clear();
                    ui->fieldPassword->clear();
                    ui->fieldRepeatPassword->clear();
                }
                else
                {
                    QMessageBox::critical(this,"Błąd","Istnieje już taki pracownik");
                }
        }
        else
        {
            QMessageBox::critical(this,"Błąd","Hasła muszą być takie same");
        }
    }






}

void Regworktime::on_GetEmployeeListButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if(db.open())
    {
        QSqlQueryModel* model = new QSqlQueryModel ;
        model->setQuery("SELECT employee_name, employee_surname FROM employee");
        model->setHeaderData(0,Qt::Horizontal,tr("Imię"));
        model->setHeaderData(1,Qt::Horizontal,tr("Nazwisko"));

        QTableView* view = new QTableView ;
        view->setSelectionBehavior(QAbstractItemView::SelectColumns);
        view->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->GetEmployeeComboBox->setModel(model);
        ui->GetEmployeeComboBox->setView(view);
    }


}



void Regworktime::on_GetEmployeeComboBox_currentIndexChanged(const QString &arg1)
{
    QString name = ui->GetEmployeeComboBox->currentText();
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query ;
    query.prepare("SELECT * FROM employee WHERE employee_name = '"+name+"'");
    if(query.exec())
    {
        while(query.next())
        {
           ui->idLineEdit->setText(query.value(0).toString());
           ui->fieldeditName->setText(query.value(1).toString());
           ui->fieldeditSurname->setText(query.value(2).toString());
           ui->fieldeditPhone->setText(query.value(3).toString());
           ui->fieldeditDate->setDate(query.value(5).toDate());
        }
    }

}

void Regworktime::on_buttonEditEmployee_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    /*Employee editEmployee(ui->idLineEdit->text().toInt(),
                          ui->fieldeditName->text(),
                          ui->fieldeditSurname->text(),
                          ui->fieldeditPhone->text(),
                          ui->fieldeditDate->date());

    QString dateFormat = "yyyy-MM-dd";

    QSqlQuery editQuery;
    editQuery.prepare("UPDATE employee SET employee_name = '"+editEmployee.name+"'"
                      ", employee_surname = '"+editEmployee.surname+"', phone_number = '"+editEmployee.phone_number+"'"
                      ",date_of_employment = '"+editEmployee.date_of_employment.toString(dateFormat)+"' WHERE employee_id = :employee_id ");
    editQuery.bindValue(":employee_id",editEmployee.id);
    if(editQuery.exec())
    {
        QMessageBox::information(this,"Sukces","Edycja danych zakończyła się powodzeniem");
    }
    else
    {
        QMessageBox::critical(this,tr("Błąd"),editQuery.lastError().text());
    }

*/

}

void Regworktime::on_GetEmployeeListschedule_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if(db.open())
    {
        QSqlQueryModel* model = new QSqlQueryModel ;
        model->setQuery("SELECT employee_name, employee_surname FROM employee");
        model->setHeaderData(0,Qt::Horizontal,tr("Imię"));
        model->setHeaderData(1,Qt::Horizontal,tr("Nazwisko"));

        QTableView* view = new QTableView ;
        view->setSelectionBehavior(QAbstractItemView::SelectColumns);
        view->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->scheduleGetEmployeeList->setModel(model);
        ui->scheduleGetEmployeeList->setView(view);
    }

}

void Regworktime::on_scheduleGetEmployeeList_currentIndexChanged(const QString &arg1)
{
    QString name = ui->scheduleGetEmployeeList->currentText();
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query ;
    query.prepare("SELECT * FROM employee WHERE employee_name = '"+name+"'");
    if(query.exec())
    {
        while(query.next())
        {
           ui->scheduleEmployeeId->setText(query.value(0).toString());
           ui->scheduleName->setText(query.value(2).toString());
           ui->scheduleSurname->setText(query.value(3).toString());
        }
    }

}

void Regworktime::on_addSchedulebutton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery scheduleQuery ;
    QSqlQuery checkQuery;
    QString dateFormat = "yyyy-MM-dd";
    QString id = ui->scheduleEmployeeId->text();
    QDate startDate = ui->workstartDate->date();
    QTime startTime = ui->workstartTime->time();
    QTime endTime = ui->workendTime->time();


   checkQuery.prepare("SELECT COUNT(work_start_date)  FROM schedule WHERE employee_id = '"+id+"' AND  work_start_date = '"+startDate.toString(dateFormat)+"'");
    checkQuery.exec();
    qDebug() << checkQuery.first();
    qDebug() << checkQuery.value("COUNT(work_start_date)").toString();
   qDebug() << "COUNT: "  << "ID: " << id << "date: " << startDate;

   //scheduleQuery.prepare("INSERT INTO schedule (employee_id, emp_employee_id,employee_status_id, work_start_date,work_start_time, work_end_time)"
   //                       "VALUES ('"+id+"','2',NULL, '"+startDate.toString(dateFormat)+"', '"+startTime.toString()+"','"+endTime.toString()+"') ");

        if(checkQuery.exec())
        {
            QMessageBox::information(this,"Sukces","Zapis w grafiku dodany poprawnie");
        }
        else
        {
            QMessageBox::critical(this,"Błąd",scheduleQuery.lastError().text());
        }




}

void Regworktime::on_editSchedulebutton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery editScheduleQuery;
    QString dateFormat = "yyyy-MM-dd";
    QString id = ui->scheduleEmployeeId->text();
    QDate startDate = ui->workstartDate->date();
    QTime startTime = ui->workstartTime->time();
    QTime endTime = ui->workendTime->time();

    editScheduleQuery.prepare("UPDATE schedule SET work_start_date = '"+startDate.toString(dateFormat)+"', work_start_time = '"+startTime.toString(dateFormat)+"' "
                              " work_end_time = '"+endTime.toString()+"' WHERE employee_id = '"+id+"' AND work_start_date = '"+startDate.toString(dateFormat)+"' ");

    if(editScheduleQuery.exec())
    {
        QMessageBox::information(this,"Sukces","Pomyślnie edytowałeś zapis w grafiku");

    }
    else
    {
        QMessageBox::critical(this,"Błąd",editScheduleQuery.lastError().text());
        qDebug() << id << " " << startDate.toString(dateFormat) << " " << startTime.toString() <<  " " << endTime.toString() ;
    }

}


void Regworktime::on_radioButtonEmployee_clicked()
{
        ui->fieldLogin->setReadOnly(true);
        ui->fieldPassword->setReadOnly(true);
        ui->fieldRepeatPassword->setReadOnly(true);
}

void Regworktime::on_radioButtonSuperior_clicked()
{
    ui->fieldLogin->setReadOnly(true);
    ui->fieldPassword->setReadOnly(false);
    ui->fieldRepeatPassword->setReadOnly(false);

}
