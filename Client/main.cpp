#include <QCoreApplication>
#include <QTimer>
#include <iostream>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client client(/*port*/0);

    QTimer timer;
    timer.setInterval(1000);
    QObject::connect(&timer, &QTimer::timeout, &client, [&client](){(&client)->sendMessage("Hello from client");});
    timer.start();

    return a.exec();
}
