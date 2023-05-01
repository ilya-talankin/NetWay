#ifndef OUTPUT_H
#define OUTPUT_H

#include <QTcpServer>
#include <QTcpSocket>

#include <QVector>

#include <iostream>

class Output : public QTcpServer
{
    Q_OBJECT

public:
    Output();
    int port = 3333; //нужно будет менять порты
    void SendToRetranslator(QString str);


private:
    bool status_sender = false;

    int best_weight = 1000;

    QTcpSocket *socket;
    QByteArray Data;

    quint16 nextBlockSize = 0;

public slots:
    void slotReadyRead();
};

#endif // OUTPUT_H
