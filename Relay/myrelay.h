#ifndef MYRELAY_H
#define MYRELAY_H

#include <QTcpServer>
#include <QtWidgets/QtWidgets>
#include <QObject>


class MyRelay : public QTcpServer
{
public:
    explicit MyRelay(const QString& arg1, const QString& arg2, QTextEdit* log, QTcpServer *parent = nullptr);
    void setArgs(const QString& arg1, const QString& arg2);
    void startServer();
    void sendData(QTcpSocket* pSocket, const QString& str);
    void sendToServer(/*QTcpSocket* pSocket, */const QString& str);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
public slots:
    void requestNewData();
    //void abortConnection();
    void readData();
    void sendMsg(const QString& data);
private:
    QTextEdit* log;
    //сокет сервера
    QTcpSocket *tcpSocket = nullptr;
    QString hostSocket;
    QTcpSocket *tcpRemoteSocket = nullptr;
    QString hostRemoteSocket;
    //QPushButton *requestButton = nullptr;

    QDataStream dataIn;
};

#endif // MYRELAY_H
