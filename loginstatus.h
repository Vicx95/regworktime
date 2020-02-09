#ifndef LOGINSTATUS_H
#define LOGINSTATUS_H
#include <QString>

class LoginStatus
{
private:
    static QString name;
    static QString surname;
    static QString id;
public:
    LoginStatus();

    QString WhoIsLoggedNow();
};

#endif // LOGINSTATUS_H
