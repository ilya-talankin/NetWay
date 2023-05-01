
#ifndef MYSERVER_H
#define MYSERVER_H

#include "qtcpsocket.h"
#include "qwidget.h"
#include <QTcpServer>
#include <QObject>
#include <QtWidgets/QtWidgets>

class QTextEdit;
class MyServer : public QTcpServer
{
//Q_OBJECT
public:
    explicit MyServer(const QStringList& arguments, QTextEdit* log, QTcpServer *parent = nullptr);
    void setArgs(const QStringList& arguments);
    void startServer();
    //void sendToClient(QTcpSocket* to, const QString& str);
signals:

public slots:
    //void startSending();
protected:
    void incomingConnection(qintptr socketDescriptor) override;
private:
    //число пакетов
    int amount{};
    //указанный в argv[] сокет
    QString hostSocket;
    QTcpSocket* toSend = nullptr;
    QTextEdit* log;
    QTcpServer *server = nullptr;
    QList<QThread*> clientThreads;
};

#endif // MYSERVER_H
