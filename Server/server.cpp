#include "server.h"

//TODO Доработать конструктор
Server::Server(quint16 port, quint16 id, QObject *parent)
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
}
void Server::onConnected()
{
    qDebug() << "New connection";
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, &QIODevice::readyRead, this, &Server::onRead);
    connect(socket, &QTcpSocket::disconnected, this, [](){qDebug("Disconnected");});
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
    in >> message;
    if (!in.commitTransaction())
        return;
    qDebug() << message << " from port " << socket->peerPort();
}


