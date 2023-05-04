#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QMap>
#include <QString>
#include "../Handshaker/Handshaker.h"

class Client : public QObject
{
    Q_OBJECT
public:
    /* В конструкторе принимаем порт, на котором будет находиться клиент
     * TODO Добавить порты, к которым нужно подключиться */
    Client(quint16 id, quint16 clientPort, const QVector<quint16>& portsVec, QObject *parent = nullptr);
    ~Client();
    quint64 sendMessage(quint16 serverId, const QString& message);
    bool connectToServer(quint16 serverPort);
    void disconnectAll(); //TODO Сделать дисконнект всех сокетов в m_serversMap
private:
    void receive( const QString& message);
signals:
    void include(quint16 ID);
    void redirect(quint16 receiverID, const QString& message);
private slots:
    /* TODO
     * Слот должен заносить сокет в m_serversMap, если получил от сервера его id
     * Слот должен либо проталкивать на предыдущий узел информацию об ошибке */
    void onRead();

private:
    /*
     * Таблица пар server id - Server socket
     * При подключении сервер отправляет клиенту свой id
     * и тот записывает id в таблицу
     */
    QMap<quint16, QTcpSocket*> m_serversMap;
    QDataStream in;
    Handshaker m_hh;
    quint16 myID;
};

#endif // CLIENT_H
