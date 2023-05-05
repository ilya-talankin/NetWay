#include <QtNetwork>
#include <QtCore>
#include "retranslator.h"

Retranslator::Retranslator(quint16 id, quint64 /*delay*/, quint16 serverPort, const QVector<quint16>& portsVec, QObject *parent)
    : QObject(parent)
{
    server = new Server(id, serverPort, this);
    client = new Client(id, portsVec, this);
    connect(server, &Server::readed, this, &Retranslator::redirect);
    connect(client, &Client::readed, this, &Retranslator::redirect);
    //connect(tcpServer, &QTcpServer::newConnection, this, &Retranslator::onConnected);
    //connect(server, &QTcpServer::newConnection, this, &Retranslator::onConnected);
}

void Retranslator::redirect(const QString &message)
{
    qDebug() << "Redir " << message;
    QString messageType = message.split('@').first();
    if (messageType == "PKG") {
        QString path = message.mid(message.indexOf("#"));
        QStringList nodes = path.split('#');
        int me = nodes.indexOf(QString::number(client->id()));
        quint16 next = nodes.at(++me).toUShort();
        client->sendMessage(next, message);
        return;
    }
    if (messageType == "SRCH") {
        QString add = '#' + QString::number(client->id());
        client->sendEverybody(message + add);
    }
}


