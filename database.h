#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QtSql>
class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

    void close_db();
    QSqlDatabase get_database();

signals:

public slots:


private:
     QSqlDatabase db;
};

#endif // DATABASE_H
