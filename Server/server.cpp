#include "server.h"

//TODO Доработать конструктор
Server::Server(quint16 id, quint16 port, QObject *parent)
    : QObject(parent), m_hh(id, this)
{
    initServer(port);
}

void Server::initServer(quint16 port)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, port)) {
        return;
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onConnected);
    in.setVersion(QDataStream::Qt_6_5);
    emit connected();
}

void Server::onConnected()
{
    qDebug() << "New connection";
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    if (!socket)
        return;
    quint16 clientID = socket->socketDescriptor();
    m_clientsMap[clientID] = socket;
    connect(socket, &QIODevice::readyRead, this, &Server::onRead);
    connect(socket, &QTcpSocket::disconnected, this, [clientID](){qDebug() << QString("ID %1 Disconnected").arg(clientID);});
    m_hh.handshake(socket);
}

void Server::onRead()
{
    QTcpSocket* socket = dynamic_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }
    QString message;
    in.setDevice(socket);
    in.startTransaction();
    if (!in.commitTransaction())
        return;
    in >> message;
    qDebug() << message << " from port " << socket->peerPort();
}


