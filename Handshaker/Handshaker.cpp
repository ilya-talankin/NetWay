#include "Handshaker.h"
Handshaker::Handshaker(quint16 id, QObject* parent)
    : QObject(parent), m_id(id)
{
}

void Handshaker::handshake(QTcpSocket *tcpSocket)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << "ID " + QString::number(m_id);
    tcpSocket->write(block);
}

void Handshaker::handshakeSlot()
{
    QTcpSocket* serverConnection = dynamic_cast<QTcpSocket*>(sender());
    if (!serverConnection) {
        return;
    }
    handshake(serverConnection);
}
