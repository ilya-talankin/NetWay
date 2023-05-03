#include <QCoreApplication>
#include "retranslator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Retranslator retranslator;
    return app.exec();
}
