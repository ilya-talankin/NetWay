#include <QCoreApplication>
#include <QTextStream>

#include <iostream>
#include <string>

#include "output.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Output output;


    return a.exec();
}
