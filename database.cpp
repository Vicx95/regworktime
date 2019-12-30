#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("Driver={MySQL ODBC 8.0 Unicode Driver};DATABASE=regworktime;");
    db.setUserName("root");
    db.setPassword("");
}

void Database::close_db()
{
    db.close();
}

QSqlDatabase Database::get_database()
{
    return this->db;
}
