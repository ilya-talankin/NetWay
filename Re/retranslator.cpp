#include <QtNetwork>
#include <QtCore>
#include "../Server/server.h"
#include "retranslator.h"

Retranslator::Retranslator(quint16 id, quint16 serverPort, const QVector<quint16>& portsVec, QObject *parent)
    : QObject(parent)
{
    server = new Server(id, serverPort, this);
    client = new Client(id, portsVec, this);
    connect(server, &Server::redirect, client, &Client::sendMessage);
    connect(client, &Client::redirect, server, &Server::sendMessage);
    //connect(tcpServer, &QTcpServer::newConnection, this, &Retranslator::onConnected);
    //connect(server, &QTcpServer::newConnection, this, &Retranslator::onConnected);
}


