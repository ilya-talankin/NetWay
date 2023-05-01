#include "myclient.h"
#include <QtWidgets/QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyClient client(a.arguments());
    client.show();
    return a.exec();
}
