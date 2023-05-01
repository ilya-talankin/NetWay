#ifndef RETRANSLATOR_H
#define RETRANSLATOR_H

#include <QTcpServer>
#include <QTcpSocket>

class Retranslator : public QTcpServer
{
    Q_OBJECT

public:
    Retranslator();
    int port_in = 3333; //нужно будет менять порты
    int port_out = 4444;
    QTcpSocket *socket_in, *socket_out;

private:
    bool status_sender = false;
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    quint16 nextBlockSize = 0;
    void SendToClientData(QString str);
    void SendToClientStatus(bool status);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyReadIn();
    void slotReadyReadOut();
};

#endif // RETRANSLATOR_H
