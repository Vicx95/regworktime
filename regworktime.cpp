#include "regworktime.h"
#include "ui_regworktime.h"

Regworktime::Regworktime(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Regworktime)
{
    ui->setupUi(this);
}

Regworktime::~Regworktime()
{
    delete ui;
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
