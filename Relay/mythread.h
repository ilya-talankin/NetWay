
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QtWidgets/QtWidgets>


class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr ID, QTextEdit* log, QObject *parent = nullptr);
    void sendToClient(/*QTcpSocket *clientSocket, */const QString& str);
    void run() override;
signals:
    void error(QTcpSocket::SocketError socketerror);
    void newMsg(const QString& str);
public slots:
    void readClient();
    void disconnected();
    //void abortConnection();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QTextEdit* m_log;
    quint16 nextBlockSize;
    QDataStream dataIn;
};

#endif // MYTHREAD_H
