#ifndef RETRANSLATOR_H
#define RETRANSLATOR_H

#include <QTcpServer>
#include "../Server/server.h"
//#include "../Server/server.cpp"
#include "../Client/client.h"
//#include "../Client/client.cpp"
#include "../Handshaker/Handshaker.h"
//#include "../Handshaker/Handshaker.cpp"

//! [0]
class Retranslator : public QObject
{
    Q_OBJECT

public:
    explicit Retranslator(quint16 id, quint16 serverPort, quint16 clientPort, const QVector<quint16>& portsVec, QObject *parent = nullptr);

private slots:
    //void onConnected();
    //void readMessage();
private:
    //void initServer();
    //QTcpServer *tcpServer = nullptr;
    //QTcpSocket *tcpSocket = nullptr;
    Server* server = nullptr;
    Client* client = nullptr;
    QDataStream in;
};
//! [0]


#endif // RETRANSLATOR_H
