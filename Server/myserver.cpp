#include "myserver.h"
#include "mythread.h"
#include "qtextedit.h"

MyServer::MyServer(const QStringList& arguments, QTextEdit* log, QTcpServer *parent) :
    QTcpServer(parent), log(log)
{
    setArgs(arguments);
    startServer();
}

void MyServer::setArgs(const QStringList& arguments)
{
    for (int count = 1; count < arguments.count(); ++count) {
        if (arguments[count].compare("-l") == 0) {
            amount = arguments[++count].toInt();
            continue;
        }
        else if (arguments[count].compare("-ip") == 0) {
            hostSocket = arguments[++count];
            continue;
        }
    }
}

void MyServer::startServer()
{
    QHostAddress ip = QHostAddress(hostSocket.split(':').first());
    int port = hostSocket.split(':').last().toInt();
    if (!this->listen(ip, port)){
        log->append("Could not start server");
        return;
    } else {
        log->append("Listening to socket " + ip.toString() + ":" + QString::number(port));
    }
}
/*
void MyServer::startSending()
{
    if (clientThreads.isEmpty()){
        log->append("Nobody to send");
        return;
    }
    sendToClient(toSend,"Ok");
}
void MyServer::sendToClient(QTcpSocket* to, const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str;
    to->write(arrBlock);
    qDebug()<<"Incoming conn";
}*/

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    //qDebug()<<"Incoming conn";
    log->append(QString::number(socketDescriptor) + " Connecting...");
    MyThread *thread = new MyThread(socketDescriptor, log, this);
    //toSend = new QTcpSocket(this);
    //toSend->setSocketDescriptor(socketDescriptor);
    //thread->socket->setSocketDescriptor(socketDescriptor);
    //toSend = thread->socket;
    connect(thread, &MyThread::finished, thread, &MyThread::deleteLater);
    thread->start();
    //clientThreads.append(thread);
}
