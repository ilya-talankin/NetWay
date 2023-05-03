#include "server.h"

//TODO Доработать конструктор
Server::Server(quint16 /*port*/, quint16 /*id*/, QObject *parent)
    : QObject(parent)
{
    initServer();
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onConnected);
    in.setVersion(QDataStream::Qt_6_5);
}

void Server::initServer()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, 3333)) {
        return;
    }
}
void Server::onConnected()
{
    qDebug() << "New connection";
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, &QTcpSocket::disconnected, this, [](){qDebug("Disconnected");});
    connect(socket, &QIODevice::readyRead, this, &Server::onRead);
    //TODO Обработать подключение, отправить свой id
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


