#ifndef EVENT_H
#define EVENT_H
#include <QString>
class Event
{
public:
    Event();

    virtual void saveEvent(const QString& superiorID, const QString& description) = 0;


    virtual ~Event();

};



#endif // EVENT_H
