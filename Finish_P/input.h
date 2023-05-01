#ifndef INPUT_H
#define INPUT_H

#include <QTcpServer>
#include <QTcpSocket>


class Input : public QTcpServer
{
    Q_OBJECT

public:
    Input();
    int port = 4444; //нужно будет менять порты
    QTcpSocket *socket;

private:
    bool status_sender = false;
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    quint16 nextBlockSize = 0;
    void SendToClient(bool status);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
};

#endif // INPUT_H
