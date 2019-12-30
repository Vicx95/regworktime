#ifndef REGWORKTIME_H
#define REGWORKTIME_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Regworktime; }
QT_END_NAMESPACE

class Regworktime : public QWidget
{
    Q_OBJECT

public:
    Regworktime(QWidget *parent = nullptr);
    ~Regworktime();

private:
    Ui::Regworktime *ui;
};
#endif // REGWORKTIME_H
