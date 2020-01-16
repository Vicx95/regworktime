#include "regworktime.h"
#include "serialportmonitor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("Driver={MySQL ODBC 8.0 Unicode Driver};DATABASE=regworktime;");
    db.setUserName("root");
    db.setPassword("");
    Regworktime w;
    w.show();

    return a.exec();
}
