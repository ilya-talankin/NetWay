#include <QtNetwork>
#include <QtCore>
#include "../Server/server.h"
#include "retranslator.h"

Retranslator::Retranslator(quint16 id, quint16 serverPort, quint16 clientPort, const QVector<quint16>& portsVec, QObject *parent)
    : QObject(parent)
{
    server = new Server(id, serverPort, this);
    client = new Client(id, clientPort, portsVec, this);
    connect(server, &Server::redirect, client, &Client::sendMessage);
    connect(client, &Client::redirect, server, &Server::sendMessage);
    //initServer();
    //connect(tcpServer, &QTcpServer::newConnection, this, &Retranslator::onConnected);
    //connect(server, &QTcpServer::newConnection, this, &Retranslator::onConnected);
}
/*
void Retranslator::initServer()
{
    tcpServer = new QTcpServer(this);

    if (!tcpServer->listen(QHostAddress("192.168.0.10"), 3333)) {
        return;
    }
    in.setVersion(QDataStream::Qt_6_5);
}
*/
/*
void Retranslator::onConnected()
{
    qDebug() << "New connection emited!";
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_6_5);
    if (!socket)
        return;

    auto readMessage = [socket, this]()
    {
        in.startTransaction();
        QString message;
        in >> message;
        qDebug() << message;
        qDebug("666");
    };
    connect(socket, &QIODevice::readyRead, this, readMessage);
}
*/
/*
void Retranslator::readMessage()
{
    in.startTransaction();

    QString message;
    in >> message;

    if (!in.commitTransaction())
        return;
    qDebug() << "Reading message ...";
    qDebug() << message;
}
*/


