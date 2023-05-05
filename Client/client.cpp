#include "client.h"

Client::Client(quint16 id, const QVector<quint16>& portsVec, QObject *parent)
    : QObject(parent), m_hh(id, this)
{
    for (quint16 serverPort : portsVec) {
        QTcpSocket* serverConnection = new QTcpSocket(this);
        serverConnection->connectToHost(QHostAddress::LocalHost, serverPort);
        connect(serverConnection, &QTcpSocket::connected, &m_hh, &Handshaker::handshakeSlot);
        connect(serverConnection, &QTcpSocket::connected, serverConnection, [serverConnection](){
            qDebug() << "Server connection on port " << serverConnection->localPort();
        });
        connect(serverConnection, &QTcpSocket::readyRead, this, &Client::onRead);
        connect(serverConnection, &QTcpSocket::disconnected, serverConnection, [serverConnection](){
            serverConnection->deleteLater();
        });
    }
    in.setVersion(QDataStream::Qt_6_5);
}

Client::~Client()
{
    disconnectAll();
}

void Client::onRead()
{
    //получаем сокет, от которого пришло сообщение
    QTcpSocket* serverConnection = dynamic_cast<QTcpSocket*>(sender());
    if (!serverConnection) {
        return;
    }

    QString message;
    in.setDevice(serverConnection);
    in.startTransaction();
    if (!in.commitTransaction())
        return;
    in >> message;
    qDebug() << message << " from port " << serverConnection->peerPort();
    /* Если посылка содержит id сервера,
     * то записываем id и сокет в m_serversMap */
    if (message.mid(0, 2) == "ID") {
        quint16 id = message.split('#').at(1).toUInt();
        if (m_serversMap.contains(id))
            return;
        include(id, serverConnection);
    }
    else if(message.mid(0, 2) == "KA")
        m_hh.handshake(serverConnection);
    else
        emit readed(message);

}
void Client::include(quint16 ID, QTcpSocket* serverConnection)
{
    qDebug() << QString("ID %1 connected").arg(ID);
    m_serversMap[ID] = serverConnection;
    connect(serverConnection, &QTcpSocket::disconnected, this, [ID, this](){
        qDebug() << QString("ID %1 disconnected").arg(ID);
        m_serversMap.remove(ID);
    });
}

quint64 Client::sendMessage(quint16 serverId, const QString& message)
{
    if (!m_serversMap.contains(serverId))
        return 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << message;
    return m_serversMap[serverId]->write(block);
}

void Client::sendEverybody(const QString &message)
{
    qDebug() << "Broadcast";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << message;
    for (QTcpSocket* socket : m_serversMap) {
        socket->write(block);
    }
}

void Client::disconnectAll()
{
    if (m_serversMap.isEmpty()) return;
    for (const auto& serverPort : qAsConst(m_serversMap)) {
        serverPort->abort();
    }
}

quint16 Client::id()
{
    return m_hh.id();
}
