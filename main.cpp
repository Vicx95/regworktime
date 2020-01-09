#include "regworktime.h"
#include "serialportmonitor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Regworktime w;
    w.show();
    return a.exec();
}
