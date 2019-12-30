#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT
public:
    explicit Person(QObject *parent = nullptr);

    virtual void add()  = 0;
    virtual void edit()  = 0;


    virtual ~Person();

signals:

public slots:
};

#endif // PERSON_H
