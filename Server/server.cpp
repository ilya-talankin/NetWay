#include "server.h"

//TODO Доработать конструктор
Server::Server(quint16 id, quint16 port, QObject *parent)
    : QObject(parent), m_hh(id, this), myID(id)
{
    initServer(port);
}

void Server::sendMessage(quint16 id, const QString &message)
{
    if (!m_clientsMap.contains(id))
        return;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    m_clientsMap[id]->write(block);
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

void Server::startSending(QTcpSocket *socket)
{
    //TODO
}

void Server::onConnected()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    if (!socket)
        return;
    connect(socket, &QIODevice::readyRead, this, &Server::onRead);
    connect(socket, &QTcpSocket::connected, &m_hh, &Handshaker::handshakeSlot);
    connect(this, &Server::include, this, [this, socket](quint16 ID){
        connect(socket, &QTcpSocket::connected, this, [ID](){qDebug() << QString("ID %1 connected").arg(ID);});
        m_clientsMap[ID] = socket;
        connect(socket, &QTcpSocket::disconnected, this, [ID, socket, this](){
            qDebug() << QString("ID %1 disconnected").arg(ID);
            socket->deleteLater();
            m_clientsMap.remove(ID);
        });
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
    /*при знакомстве handshaker всегда отправляет сообщение со словом ID,
    если сообщение содержит его, то сигналом include
    включаем id (message.split(" ").last().toUShort()) в мап клиентов*/
    if (message.contains("ID"))
        emit include(message.split(" ").last().toUShort());
    /*если получаем сообщение от клиента со словом Ready, то если
    указанный в сообщении ID совпадает с нашим (сообщение попало на сервер),
    начинаем отправку пакетов,
    иначе (сообщение попало в ретранслятор) перенаправляем сообщение
    предполагаемая структура ready-сообщения: id:Ready, где
    id - id сервера, которому шлем запрос на пересылку пакетов*/
    else if (message.contains("Ready")) {
        quint16 receiverID = message.split(":").first().toUInt();
        if (receiverID == myID)
            startSending(socket);
        else emit redirect(receiverID, message);
    }
    /*в общем случае (для первой версии) просто перенаправляем пакет
    в метод sengMessage клиента*/
    else {
        quint16 receiverID = message.split(":").first().toUInt();
        emit redirect(receiverID, message);
    }
    qDebug() << message << " from port " << socket->peerPort();
}


