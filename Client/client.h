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
    Client(quint16 id, const QVector<quint16>& portsVec, QObject *parent = nullptr);
    ~Client();
    quint64 sendMessage(quint16 serverId, const QString& message);
    void sendEverybody(const QString& message);
    void disconnectAll(); //TODO Сделать дисконнект всех сокетов в m_serversMap
    quint16 id();
private:
    void include(quint16 ID, QTcpSocket* serverConnection);
signals:
    void readed(const QString& message);
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
};

#endif // CLIENT_H
