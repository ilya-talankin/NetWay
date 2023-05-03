#include "client.h"

Client::Client(quint16 clientPort, const QVector<quint16>& portsVec, QObject *parent)
    : QObject(parent)
{
    /* TODO  Создать сокет для каждого сервера
     * Привязать сигнал ReadyRead к слоту readyRead() */
    //for .... Перебираем порты из аргумента

    for (int i = 0; i < portsVec.size(); i++)
    {
        QTcpSocket* serverConnection = new QTcpSocket(this);
        serverConnection->bind(QHostAddress::LocalHost, clientPort);
        serverConnection->connectToHost(QHostAddress::LocalHost, portsVec[i]); //!
        connect(serverConnection, &QTcpSocket::readyRead, this, &Client::onRead);
        connect(serverConnection, &QTcpSocket::disconnected, serverConnection, &QObject::deleteLater);
    }
}

Client::~Client()
{
    disconnectAll();
}
void Client::onRead()
{
    /* Если посылка содержит id сервера,
     * то записываем id и сокет в m_serversMap */
    //получаем сокет, от которого пришло сообщение
    /*QTcpSocket* serverConnection = dynamic_cast<QTcpSocket*>(sender());
    if (!serverConnection) {
        return;
    }
    m_serversMap[serverId] = serverConnection;*/

    QTcpSocket* serverConnection = dynamic_cast<QTcpSocket*>(sender());
    QDataStream in(serverConnection);
    in.setVersion(QDataStream::Qt_6_4);
    if (!serverConnection) {
        return;
    }

    QString message;

    in >> message;
    //парсим сообщение
    QStringList id_from_message = message.split("#"); //пусть после id добавляется спецсимвол #
    bool ok;
    int serverId = id_from_message[0].toInt(&ok,10);
    if(ok == true)
    {
        m_serversMap[serverId] = serverConnection;
    }
    else
    {
        qDebug() << "Recieve message is: " << message;
    }


    // если сообщение об ошибке, то проталкиваем его дальше согласно маршруту

}

quint64 Client::sendMessage(quint16 serverId, const QString& message)
{
    if (!m_serversMap.contains(serverId))
        return 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << message;
    return m_serversMap[serverId]->write(block);
}

void Client::disconnectAll()
{
    for(int i = 0; i < m_serversMap.size(); i++)
    {
        m_serversMap[i]->disconnectFromHost();
    }
}

