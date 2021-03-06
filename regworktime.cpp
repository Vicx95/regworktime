﻿#include "regworktime.h"
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
    ui->loginStatus->setVisible(false);
    ui->backtoMenu->setVisible(false);
    ui->stackedWidget->setCurrentIndex(0);

    ui->workstartDate->setDate(QDate::currentDate());
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
        QMessageBox::critical(this,tr("Błąd"),"Nie można odnaleźć urządzenia...");
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


void Regworktime::registry(const QString& employeeID)
{




}

void Regworktime::readData()
{

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery checkCardIDQuery;

    QString employee_id ;
    QString cardIDfromDB;


    const QByteArray data = serialPortMonitor->readAll();
    console->putData(data);


    QString id_from_card = QString::number(data.toInt());

    checkCardIDQuery.prepare("SELECT employee_id, employee_card_id FROM employee WHERE employee_card_id = '"+id_from_card+"'");
    checkCardIDQuery.exec();
       checkCardIDQuery.next();
       employee_id = checkCardIDQuery.value("employee_id").toString();
       cardIDfromDB = checkCardIDQuery.value("employee_card_id").toString();


       qDebug() << employee_id << " " << id_from_card  << " " << cardIDfromDB << " " ;


      // registry(employee_id);
       QString dateFormat = "yyyy-MM-dd";
       QSqlQuery checkScheduleQUery;
       QSqlQuery getNumberOfCardReadings;

       QString schedule_id ;
       QString employee_status_id ;
       QDate todayDate = QDate::currentDate();
       checkScheduleQUery.prepare("SELECT schedule_id, employee_status_id FROM employee_schedule WHERE employee_id = '"+employee_id+"' AND work_start_date = '"+todayDate.toString(dateFormat)+"' ");


       qDebug() << todayDate.toString(dateFormat) ;

       if(checkScheduleQUery.exec())
       {
           while(checkScheduleQUery.next())
           {
               schedule_id = checkScheduleQUery.value(0).toString();
               employee_status_id = checkScheduleQUery.value(1).toString();
           }
       }

      getNumberOfCardReadings.prepare("SELECT number_of_card_readings FROM employee_status WHERE schedule_id = '"+schedule_id+"' AND employee_status_id = '"+employee_status_id+"'");
      int number_of_card_readingsFromDB = 0;

      if(getNumberOfCardReadings.exec())
      {
          while(getNumberOfCardReadings.next())
          {
              number_of_card_readingsFromDB = getNumberOfCardReadings.value(0).toInt();
          }
      }

              qDebug() << " schedule id: " << schedule_id << "status: " << employee_status_id << "card readings: " << number_of_card_readingsFromDB;

    QSqlQuery updateStatus;
    QString timeFormat = "hh:mm";
    QTime getTime = QTime::currentTime();

     switch(number_of_card_readingsFromDB)
     {
        case 0 :
        {
           updateStatus.prepare("UPDATE employee_status SET employee_entrance_time = '"+getTime.toString(timeFormat)+"' "
           ", number_of_card_readings = '1', is_present = '1' WHERE schedule_id = '"+schedule_id+"' AND employee_status_id = '"+employee_status_id+"' AND date = '"+todayDate.toString(dateFormat)+"'");
           updateStatus.exec();
           break;
        }
        case 1:
        {
            updateStatus.prepare("UPDATE employee_status SET break_start_time = '"+getTime.toString(timeFormat)+"' , number_of_card_readings = '2' "
            "WHERE schedule_id = '"+schedule_id+"' AND employee_status_id = '"+employee_status_id+"' AND date = '"+todayDate.toString(dateFormat)+"' ");
            updateStatus.exec();
            break;
        }
        case 2:
        {
            updateStatus.prepare("UPDATE employee_status SET break_end_time = '"+getTime.toString(timeFormat)+"' , number_of_card_readings = '3' "
            "WHERE schedule_id = '"+schedule_id+"' AND employee_status_id = '"+employee_status_id+"' AND date = '"+todayDate.toString(dateFormat)+"' ");
            updateStatus.exec();
            break;
        }
        case 3:
        {
            updateStatus.prepare("UPDATE employee_status SET employee_departure_time = '"+getTime.toString(timeFormat)+"' , number_of_card_readings = '4' "
            "WHERE schedule_id = '"+schedule_id+"' AND employee_status_id = '"+employee_status_id+"' AND date = '"+todayDate.toString(dateFormat)+"' ");
            updateStatus.exec();
            break;
        }
     }



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
         ui->loginStatus->setVisible(true);
         ui->loginStatus->setText(superior_login);
         ui->backtoMenu->setVisible(true);
         ui->UserloginField->clear();
         ui->PasswordField->clear();
     }
     else
     {
        QMessageBox::critical(this,"Błąd","Nieprawidłowa nazwa użytkownika lub hasło");
        ui->stackedWidget->setCurrentIndex(0);
     }
}


void Regworktime::on_LogoutButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->loginStatus->clear();
    ui->backtoMenu->setVisible(false);
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
    if(db.open())
    {
        QSqlQueryModel *model = new QSqlQueryModel();

        QSqlQuery query;
        query.prepare("SELECT * FROM check_presence");

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
        QString superiorID;
        QString login = ui->loginStatus->text() ;
        QSqlQuery getSuperiorLoginQuery;
        getSuperiorLoginQuery.prepare("SELECT employee_id from employee WHERE superior_login = '"+login+"'");

        if(getSuperiorLoginQuery.exec())
        {
            while(getSuperiorLoginQuery.next())
            {
                superiorID = getSuperiorLoginQuery.value(0).toString();
            }
        }

        if(employee.name.isEmpty() || employee.surname.isEmpty() || employee.phone_number.isEmpty() || employee.card_number.isEmpty() )
        {
            QMessageBox::critical(this,"Błąd","Wszystkie pola muszą być wypełnione");
        }
        else
        {
            if(db.open())
            {

                QSqlQuery addEmployeeQuery;
                addEmployeeQuery.prepare("INSERT INTO employee SET emp_employee_id = '"+superiorID+"' , employee_name = '"+employee.name+"', employee_surname = '"+employee.surname+"', employee_phone ='"+employee.phone_number+"',"
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
        if(superior.superior_name.isEmpty() || superior.superior_surname.isEmpty()||
           superior.phone_number.isEmpty() || superior.card_number.isEmpty()||
           superior.getPassword().isEmpty() || superior.getRepeatPassword().isEmpty()
           )
        {
            QMessageBox::critical(this,"Błąd","Wszystkie pola muszą być wypełnione");
        }
        else
        {
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
           ui->fieldeditName->setText(query.value(2).toString());
           ui->fieldeditSurname->setText(query.value(3).toString());
           ui->fieldeditPhone->setText(query.value(4).toString());
           ui->fieldeditCardID->setText(query.value(5).toString());
        }
    }
}

void Regworktime::on_buttonEditEmployee_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

   Employee editEmployee(ui->idLineEdit->text(),
                          ui->fieldeditName->text(),
                          ui->fieldeditSurname->text(),
                          ui->fieldeditPhone->text(),
                          ui->fieldeditCardID->text()
                          );
    QSqlQuery editQuery;
    QSqlQuery eventQuery;
    QString superiorID;
    QString login = ui->loginStatus->text() ;
    eventQuery.prepare("SELECT employee_id from employee WHERE superior_login = '"+login+"'");

    if(eventQuery.exec())
    {
        while(eventQuery.next())
        {
            superiorID = eventQuery.value(0).toString();
        }
    }
     QString date = QDate::currentDate().toString();
     QString superior = ui->loginStatus->text();
     QString employee_id = editEmployee.id;
    QString description = "Przełożony " + superior + " edytował dane pracownika o id " + employee_id + " dnia " + date ;

    EditRegisterEvent editEvent ;

    editQuery.prepare("UPDATE employee SET employee_name = '"+editEmployee.name+"'"
                      ", employee_surname = '"+editEmployee.surname+"', employee_phone = '"+editEmployee.phone_number+"'"
                      ",employee_card_id = '"+editEmployee.card_number+"' WHERE employee_id = '"+editEmployee.id+"'");
    if(editQuery.exec())
    {
        QMessageBox::information(this,"Sukces","Edycja danych zakończyła się powodzeniem");
        editEvent.saveEvent(superiorID,description);
    }
    else
    {
        QMessageBox::critical(this,tr("Błąd"),"Istnieje już taki pracownik");
    }
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
    QString dateFormat = "yyyy-MM-dd";
    QString id = ui->scheduleEmployeeId->text();
    QDate startDate = ui->workstartDate->date();
    QTime startTime = ui->workstartTime->time();
    QTime endTime = ui->workendTime->time();
    QDate currentDate = QDate::currentDate();

    if(startDate < currentDate)
    {
         QMessageBox::critical(this,"Błąd","Niepoprawna data");
    }
    else
    {
        QSqlQuery employeeStatusQuery;
        QSqlQuery employeeStatusIDUpdate;
        scheduleQuery.prepare("INSERT INTO employee_schedule (employee_id, emp_employee_id,employee_status_id, work_start_date,work_start_time, work_end_time)"
                         "VALUES ('"+id+"','12',NULL, '"+startDate.toString(dateFormat)+"', '"+startTime.toString()+"','"+endTime.toString()+"') ");
            if(scheduleQuery.exec())
            {
                employeeStatusQuery.prepare("INSERT INTO employee_status SET schedule_id = (SELECT schedule_id FROM employee_schedule WHERE employee_id = '"+id+"' AND work_start_date = '"+startDate.toString(dateFormat)+"')"
                                       ",is_present = '0' , date = '"+startDate.toString(dateFormat)+"' ");
                employeeStatusQuery.exec();
                employeeStatusIDUpdate.prepare("UPDATE employee_schedule SET employee_status_id = "
                                      "(SELECT employee_status.employee_status_id FROM employee_schedule INNER JOIN employee_status ON "
                                      " employee_schedule.schedule_id = employee_status.schedule_id "
                                      " WHERE employee_schedule.work_start_date = '"+startDate.toString(dateFormat)+"' AND employee_schedule.employee_id = '"+id+"') "
                                      " WHERE employee_schedule.employee_id = '"+id+"' AND employee_schedule.work_start_date = '"+startDate.toString(dateFormat)+"'");
                employeeStatusIDUpdate.exec();
                qDebug() << startDate.toString(dateFormat);
                QMessageBox::information(this,"Sukces","Zapis w grafiku dodany poprawnie");
            }
            else
            {
                QMessageBox::critical(this,"Błąd","Pracownik ma już ustawiony grafik na ten dzień");
            }
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

    QString superiorID;
    QString login = ui->loginStatus->text() ;
    QSqlQuery eventEditSchedule;
    eventEditSchedule.prepare("SELECT employee_id from employee WHERE superior_login = '"+login+"'");

    if(eventEditSchedule.exec())
    {
        while(eventEditSchedule.next())
        {
            superiorID = eventEditSchedule.value(0).toString();
        }
    }
     QString date = QDate::currentDate().toString();
     QString superior = ui->loginStatus->text();

    QString description = "Przełożony " + superior + " edytował grafik pracownikowi o id " + id + " dnia " + date ;

    EditRegisterEvent editEvent ;

    editScheduleQuery.prepare("UPDATE employee_schedule SET  work_start_time = '"+startTime.toString()+"' "
                              ", work_end_time = '"+endTime.toString()+"' WHERE employee_id = '"+id+"' AND work_start_date = '"+startDate.toString(dateFormat)+"' ");

    if(editScheduleQuery.exec())
    {
        QMessageBox::information(this,"Sukces","Pomyślnie edytowałeś zapis w grafiku");
        editEvent.saveEvent(superiorID,description);

    }
    else
    {
        QMessageBox::critical(this,"Błąd",editScheduleQuery.lastError().text());

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

void Regworktime::on_backtoMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Regworktime::on_manualRegisterButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void Regworktime::on_addManualRegister_clicked()
{

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery manualRegisterQuery ;
    manualRegisterQuery.prepare("UPDATE employee_status SET employee_entrance_time = '"+ui->workstartTimemanualRegister->text()+"'"
                                ", employee_departure_time = '"+ui->workendTimemanualRegister->text()+"'"
                                ", number_of_card_readings = '4', break_start_time = '"+ui->breakStartTime->text()+"' "
                                ", break_end_time = '"+ui->breakEndTime->text()+"', is_present = '1' "
                                "WHERE date = '"+ui->workstartDatemanualRegister->text()+"'");

    QString employee_id = ui->manualRegisterId->text();
    QString reason = ui->registerReason->toPlainText() ;
    QString superiorID;
    QString login = ui->loginStatus->text() ;
    QSqlQuery eventEditSchedule;
    eventEditSchedule.prepare("SELECT employee_id from employee WHERE superior_login = '"+login+"'");

    if(eventEditSchedule.exec())
    {
        while(eventEditSchedule.next())
        {
            superiorID = eventEditSchedule.value(0).toString();
        }
    }
     QString date = QDate::currentDate().toString();
     QString superior = ui->loginStatus->text();

    QString description = "Przełożony " + superior + " zarejestrował czas pracy ręcznie pracownikowi o id " + employee_id + " dnia " + date  + " Powód: " + reason;

    EditRegisterEvent editEvent ;

    if(manualRegisterQuery.exec())
    {
        QMessageBox::information(this,"Sukces","Poprawnie zarejestrowałeś czas pracy");
        editEvent.saveEvent(superiorID,description);
    }
    else
    {
        QMessageBox::critical(this,"Błąd", manualRegisterQuery.lastError().text());
    }

}

void Regworktime::on_GetEmployeeListManualRegister_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if(db.open())
    {
        QSqlQueryModel* model2 = new QSqlQueryModel ;
        model2->setQuery("SELECT employee_name, employee_surname FROM employee");
        model2->setHeaderData(0,Qt::Horizontal,tr("Imię"));
        model2->setHeaderData(1,Qt::Horizontal,tr("Nazwisko"));

        QTableView* view2 = new QTableView ;
        view2->setSelectionBehavior(QAbstractItemView::SelectColumns);
        view2->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->manualRegisterGetEmployeeList->setModel(model2);
        ui->manualRegisterGetEmployeeList->setView(view2);
    }
}

void Regworktime::on_manualRegisterGetEmployeeList_currentIndexChanged(const QString &arg1)
{
    //QString id = ui->manualRegisterGetEmployeeList->currentText();
    QString name = ui->manualRegisterGetEmployeeList->currentText();
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query ;
    query.prepare("SELECT * FROM employee WHERE employee_name = '"+name+"'");
    if(query.exec())
    {
        while(query.next())
        {
           ui->manualRegisterId->setText(query.value(0).toString());
           ui->manualRegisterName->setText(query.value(2).toString());
           ui->manualRegisterSurname->setText(query.value(3).toString());
           ui->workstartDatemanualRegister->setDate(QDate::currentDate());
           ui->workstartTimemanualRegister->setTime(QTime(6,0,0));
           ui->workendTimemanualRegister->setTime(QTime(14,0,0));
           ui->breakStartTime->setTime(QTime(10,0,0));
           ui->breakEndTime->setTime(QTime(10,30,0));
        }
    }

}

void Regworktime::on_getEmployeeListGenerateSchedule_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if(db.open())
    {
        QSqlQueryModel* model2 = new QSqlQueryModel ;
        model2->setQuery("SELECT employee_name, employee_surname FROM employee");
        model2->setHeaderData(0,Qt::Horizontal,tr("Imię"));
        model2->setHeaderData(1,Qt::Horizontal,tr("Nazwisko"));

        QTableView* view2 = new QTableView ;
        view2->setSelectionBehavior(QAbstractItemView::SelectColumns);
        view2->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->generateScheduleEmployeeList->setModel(model2);
        ui->generateScheduleEmployeeList->setView(view2);
    }
}

void Regworktime::on_generateScheduleEmployeeList_currentIndexChanged(const QString &arg1)
{
    QString name = ui->generateScheduleEmployeeList->currentText();
    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query ;
    query.prepare("SELECT * FROM employee WHERE employee_name = '"+name+"'");
    if(query.exec())
    {
        while(query.next())
        {
           ui->generateID->setText(query.value(0).toString());
           ui->generateScheduleName->setText(query.value(2).toString());
           ui->generateScheduleSurname->setText(query.value(3).toString());
           ui->dateStart->setDate(QDate::currentDate());
           ui->dateEnd->setDate(QDate::currentDate());
        }
    }
}

void Regworktime::on_generateButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString timeFormat = "hh:mm";

    QVector<QString> scheduleDates;
    QVector<QString> scheduleStartTimes;
    QVector<QString> scheduleEndTimes;

    QString startRange = ui->dateStart->text();
    QString endRange = ui->dateEnd->text();

    QString employeeId = ui->generateID->text();

    QString currentYear = QString::number(QDate::currentDate().year());
    QString currentMonth = QString::number(ui->dateStart->date().month());


      QSqlQueryModel* model = new QSqlQueryModel ;
      model->setQuery("SELECT work_start_date, time_format(work_start_time,'%H:%i'), time_format(work_end_time,'%H:%i') FROM employee_schedule WHERE employee_id = '"+employeeId+"' AND work_start_date BETWEEN '"+startRange+"' AND '"+endRange+"'");
      model->setHeaderData(0,Qt::Horizontal,tr("Data"));
      model->setHeaderData(1,Qt::Horizontal,tr("Czas rozpoczęcia pracy"));
      model->setHeaderData(2,Qt::Horizontal,tr("Czas zakończenia pracy"));


      QTableView* view = new QTableView ;
        view->setModel(model);


        QString strStream;
            QTextStream out(&strStream);

            const int rowCount = view->model()->rowCount();
            const int columnCount = view->model()->columnCount();


            out <<  "<html>\n"
                "<head>\n"
                "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                <<  QString("<title>%1</title>\n").arg("Grafik")
                <<  "</head>\n"
                "<body bgcolor=#ffffff link=#5000A0>\n"
                "<table border=1 cellspacing=0 cellpadding=2 align= center font-size: 20px> margin-left=50";

            //information
            out << "<h1 align= center>Grafik pracowniczy</h1>"    ;
            out << "<div align = center> "
                << QString("Imię i nazwisko: %1 %2<br>").arg(ui->generateScheduleName->text(),ui->generateScheduleSurname->text())
                <<  QString("Rok: %1<br>").arg(currentYear)
                << QString("Miesiąc: %1<br>").arg(currentMonth)
                << QString("Wygenerowano: %1<br>").arg(QDateTime::currentDateTime().toLocalTime().toString())
                <<"</div>" ;



            // headers
            out << "<thead><tr bgcolor=#f0f0f0>";
            for (int column = 0; column < columnCount; column++)
                if (!view->isColumnHidden(column))
                    out << QString("<th>%1</th>").arg(view->model()->headerData(column, Qt::Horizontal).toString());
            out << "</tr></thead>\n";

            // data table
            for (int row = 0; row < rowCount; row++) {
                out << "<tr>";
                for (int column = 0; column < columnCount; column++) {
                    if (!view->isColumnHidden(column)) {
                        QString data = view->model()->data(view->model()->index(row, column)).toString().simplified();
                        out << QString("<td bkcolor=0  align= center>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                    }
                }
                out << "</tr>\n";
            }
            out <<  "</table>\n"
                "</body>\n"
                "</html>\n";

            QTextDocument *document = new QTextDocument();
            document->setHtml(strStream);

            QPrinter printer;
            printer.setPageMargins(QMarginsF(10,10,10,10));
            printer.setPaperSize(QPrinter::A4);


            QPrintDialog *dialog = new QPrintDialog(&printer,nullptr);
            if (dialog->exec() == QDialog::Accepted) {
                document->print(&printer);
            }

            delete document;



}
