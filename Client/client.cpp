#include "client.h"

Client::Client(quint16 id, const QVector<quint16>& portsVec, QObject *parent)
    : QObject(parent), m_hh(id, this), myID(id)
{
    /* TODO  Создать сокет для каждого сервера
     * Привязать сигнал ReadyRead к слоту readyRead() */
    //qDebug() << portsVec.size();
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
    // если сообщение об ошибке, то проталкиваем его дальше согласно маршруту
    QString message;
    in.setDevice(serverConnection);
    in.startTransaction();
    if (!in.commitTransaction())
        return;
    in >> message;
    qDebug() << message << " from port " << serverConnection->peerPort();
    /* Если посылка содержит id сервера,
     * то записываем id и сокет в m_serversMap */
    if (message.contains("ID"))
        include(message.split(" ").last().toUShort(), serverConnection);

    /*если посылка - пересылаемый пакет, то
    если мы - получатель (message.contains(myID)), записываем пакет,
    иначе (мы - ретранслятор), пересылаем его серверу
    предполагаемая структура пакета: id:PCKT<номер>:timestamp:data,
    разделять строку сплитами(":"), используя first()/last(),
    timestamp = QTime::currentTime().msecsSinceStartOfDay()*/
    else if (message.contains("PCKT")) {
        quint16 receiverID = message.split(":").first().toUInt();
        if (receiverID == myID)
            receive(message.split(":").last());
        else emit redirect(receiverID, message);
    }
    /*в общем случае (для первой версии) просто перенаправляем пакет
    в метод sengMessage сервера*/
    else {
        quint16 receiverID = message.split(":").first().toUInt();
        emit redirect(receiverID, message);
    }
}
void Client::include(quint16 ID, QTcpSocket* serverConnection)
{
    qDebug() << QString("ID %1 connected").arg(ID);
    m_serversMap[ID] = serverConnection;
    qDebug() << m_serversMap;
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

void Client::disconnectAll()
{
    if (m_serversMap.isEmpty()) return;
    for (const auto& serverPort : qAsConst(m_serversMap)) {
        serverPort->abort();
    }
}

void Client::receive(const QString &message)
{
    //TODO
}


