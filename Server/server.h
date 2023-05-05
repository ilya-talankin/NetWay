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
    explicit Server(quint16 id, quint16 port, QObject *parent = nullptr);
    void sendMessage(quint16 clientId, const QString& message);
    void init(quint16 port);
private:
    void include(quint16 ID, QTcpSocket* clientConnection);
signals:
    void readed(const QString& message);
private slots:
    void onConnected();
    //TODO Подумать, как возвращать полученные данные классу, который испульзует Server
    //Как вариант, записывать их в буффер и при каждом его обновлении испускать сигнал
    void onRead();
private:
    QTcpServer *tcpServer = nullptr;
    QDataStream in;
    Handshaker m_hh;
    QMap<quint16, QTcpSocket*> m_clientsMap;
};
//! [0]
#endif // SERVER_H
