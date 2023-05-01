#include <QCoreApplication>

#include "input.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Input input;
    return a.exec();
}
