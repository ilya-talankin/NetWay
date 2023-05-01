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
    explicit MyThread(qintptr ID, QTextEdit* log, QObject *parent = 0);
    void sendToClient(const QString& str);
    void run() override;
QTcpSocket *socket;
signals:
    void error(QTcpSocket::SocketError socketerror);
void newMsg(const QString& str);
public slots:
    void readClient();
    void disconnected();
    void hello();

private:
    QDataStream dataIn;

    qintptr socketDescriptor;
    QTextEdit* m_log;
    quint16 nextBlockSize;
};

#endif // MYTHREAD_H
