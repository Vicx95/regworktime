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

