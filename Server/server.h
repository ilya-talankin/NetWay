#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QDataStream>
#include <QTcpServer>
#include <iostream>
#include "../Handshaker/Handshaker.h"

//! [0]
class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(quint16 port, quint16 id, QObject *parent = nullptr);

private slots:
    void onConnected();
    //TODO Подумать, как возвращать полученные данные классу, который испульзует Server
    //Как вариант, записывать их в буффер и при каждом его обновлении испускать сигнал
    void onRead();
private:
    void initServer(quint16 port);
    QTcpServer *tcpServer = nullptr;
    QDataStream in;
    Handshaker m_hh;
};
//! [0]
#endif // SERVER_H
