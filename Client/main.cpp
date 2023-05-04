#include <QCoreApplication>
#include <QTimer>
#include <iostream>
#include <string>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc < 4) {
        qDebug() << "Invalid arguments";
        return 0;
    }
    quint16 id;
    quint16 port;
    QVector<quint16> portsVec;
    try {
        id = std::stoi(argv[1]);
        port = std::stoi(argv[2]);
        for (int i = 3; i < argc; ++i){
            portsVec.push_back(std::stoi(argv[i]));
        }
    }
    catch(std::exception& ex) {
        qDebug() << ex.what();
    }
    Client client(id, port, portsVec);

    QTimer timer;
    timer.setInterval(1000);
    QObject::connect(&timer, &QTimer::timeout, &client, [&client, portsVec](){
        for (const auto& ports : portsVec)
            (&client)->sendMessage(ports, "Hello from client");
    });
    timer.start();

    return a.exec();
}
