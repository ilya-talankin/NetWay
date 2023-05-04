#include "client.h"

Client::Client(quint16 id, quint16 clientPort, const QVector<quint16>& portsVec, QObject *parent)
    : QObject(parent), m_hh(id, this)
{
    /* TODO  Создать сокет для каждого сервера
     * Привязать сигнал ReadyRead к слоту readyRead() */
    //qDebug() << portsVec.size();
    for (quint16 serverPort : portsVec) {
        QTcpSocket* serverConnection = new QTcpSocket(this);
        serverConnection->bind(QHostAddress::LocalHost, clientPort);
        qDebug() << "Bind to " << clientPort;
        serverConnection->connectToHost(QHostAddress::LocalHost, serverPort);
        connect(serverConnection, &QTcpSocket::readyRead, this, &Client::onRead);
        connect(serverConnection, &QTcpSocket::connected, &m_hh, &Handshaker::handshakeSlot);
        connect(serverConnection, &QTcpSocket::disconnected, serverConnection, &QObject::deleteLater);
        m_serversMap[serverPort] = serverConnection; //!!!
    }
    in.setVersion(QDataStream::Qt_6_5);
}

Client::~Client()

{
    //desconnectAll();
    if (m_serversMap.isEmpty()) return;
    for (const auto& serverPort : qAsConst(m_serversMap)) {
        serverPort->abort();
    }
}
void Client::onRead()
{
    /* Если посылка содержит id сервера,
     * то записываем id и сокет в m_serversMap */
    //получаем сокет, от которого пришло сообщение
    QTcpSocket* serverConnection = dynamic_cast<QTcpSocket*>(sender());
    if (!serverConnection) {
        return;
    }
    //m_serversMap[/*serverId*/] = serverConnection;

    // если сообщение об ошибке, то проталкиваем его дальше согласно маршруту
    QString message;
    in.setDevice(serverConnection);
    in.startTransaction();
    if (!in.commitTransaction())
        return;
    in >> message;
    qDebug() << message << " from port " << serverConnection->peerPort();
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

