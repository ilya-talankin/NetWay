#ifndef HANDSHAKER_H
#define HANDSHAKER_H

#include <QObject>
#include <QTcpSocket>

class Handshaker : public QObject
{
public:
    Handshaker(quint16 id, QObject* parent = nullptr);
    void handshake(QTcpSocket* tcpSocket);
public slots:
    void handshakeSlot();
private:
    quint16 m_id = 0;
};

#endif
