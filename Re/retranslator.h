#ifndef RETRANSLATOR_H
#define RETRANSLATOR_H

#include <QTcpServer>

//! [0]
class Retranslator : public QObject
{
    Q_OBJECT

public:
    explicit Retranslator(QObject *parent = nullptr);

private slots:
    void onConnected();
    void readMessage();
private:
    void initServer();
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
};
//! [0]


#endif // RETRANSLATOR_H
