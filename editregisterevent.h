#pragma once
#include "event.h"
#include <QSqlDatabase>
#include <QSqlQuery>
class EditRegisterEvent : public Event
{
public:
    EditRegisterEvent();

    void saveEvent(const QString &superiorID, const QString &description) override;

    ~EditRegisterEvent() override;
};
