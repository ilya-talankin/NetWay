#include "server.h"

Server::Server(quint16 id, quint16 port, QObject *parent)
    : QObject(parent), m_hh(id, this)
{
    init(port);
}

void Server::sendMessage(quint16 id, const QString &message)
{
    if (!m_clientsMap.contains(id))
        return;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << message;
    out.setVersion(QDataStream::Qt_6_5);
    m_clientsMap[id]->write(block);
}

void Server::init(quint16 port)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, port)) {
        return;
    }
    qDebug() << "Listening port " << tcpServer->serverPort();
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onConnected);

    in.setVersion(QDataStream::Qt_6_5);
}


void Server::onConnected()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    if (!socket)
        return;
    connect(socket, &QIODevice::readyRead, this, &Server::onRead);
    m_hh.handshake(socket);
}

void Server::include(quint16 ID, QTcpSocket *clientConnection)
{
    qDebug() << QString("ID %1 connected").arg(ID);
    m_clientsMap[ID] = clientConnection;
    connect(clientConnection, &QTcpSocket::disconnected, this, [ID, this](){
        qDebug() << QString("ID %1 disconnected").arg(ID);
        m_clientsMap.remove(ID);
    });
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
    emit readed(message);
}

