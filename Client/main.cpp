#include <QCoreApplication>
#include <QTimer>
#include <iostream>
#include <string>
#include "client.h"
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc < 3) {
        qDebug() << "Invalid arguments";
        return 0;
    }
    quint16 id;
    QVector<quint16> portsVec;
    try {
        id = std::stoi(argv[1]);
        for (int i = 2; i < argc; ++i){
            portsVec.push_back(std::stoi(argv[i]));
        }
    }
    catch(std::exception& ex) {
        qDebug() << ex.what();
    }
    Client client(id, portsVec);
/*
    QString message = "PKG@0334:Hello!!!#2#1";
    QStringList l1 = message.split("@");
    QString dataAndPath = l1.at(1);
    QStringList l2 = dataAndPath.split("#");
    qDebug() << l2;
    return 0;
*/
    QTimer timer;
    timer.setInterval(1000);
    QObject::connect(&timer, &QTimer::timeout, &client, [&client](){
        (&client)->sendMessage(2, "PKG@0334:Hello!!!#5#1");
    });
    timer.start();

    QTimer timer2;
    timer2.setInterval(1000);
    QObject::connect(&timer2, &QTimer::timeout, &client, [&client](){
        (&client)->sendMessage(3, "PKG@0334:Hello!!!#5#1");
    });
    timer2.start();

    return a.exec();
}
