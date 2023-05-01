#include <QCoreApplication>

#include "retranslator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Retranslator r;
    return a.exec();
}
