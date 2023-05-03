#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QDataStream>
#include <QTcpServer>
#include <iostream>

//! [0]
class Server : public QObject
{
    Q_OBJECT

public:
    // TODO Сделать конструктор, в который передаётся адрес
    explicit Server(quint16 port, quint16 id, QObject *parent = nullptr);
signals:

private slots:
    void onConnected();
    //TODO Подумать, как возвращать полученные данные классу, который испульзует Server
    //Как вариант, записывать их в буффер и при каждом его обновлении испускать сигнал
    void onRead();
private:
    void initServer();
    QTcpServer *tcpServer = nullptr;
    QDataStream in;
    quint16 id = 0;
};
//! [0]
#endif // SERVER_H
