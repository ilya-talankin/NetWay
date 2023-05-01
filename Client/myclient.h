
#ifndef MYCLIENT_H
#define MYCLIENT_H


#include "qtcpsocket.h"
#include <QWidget>
#include <QTcpServer>
#include <QtWidgets/QtWidgets>

class MyClient : public QWidget
{
    Q_OBJECT
public:
    explicit MyClient(const QStringList& arguments, QWidget *parent = nullptr);
    void setArgs(const QStringList& arguments);
    void sendToServer(QTcpSocket* pSocket, const QString& str);
signals:

private slots:
    void requestNewData();
    void abortConnection();
    void readData();
    //void hi();

private:
    int amount;
    QTextEdit* log;
    //сокет сервера
    //QVector<QTcpSocket*> mySockets;
    //QVector<QTcpSocket> hostSockets;
    QTcpSocket* tcpSocket = nullptr;
    QTcpSocket connectTo;
    QPushButton *requestButton = nullptr;
    QPushButton *stopButton = nullptr;
    QPushButton *showButton = nullptr;
    QDataStream dataIn;
    QLabel* speed = nullptr;
};

#endif // MYCLIENT_H
