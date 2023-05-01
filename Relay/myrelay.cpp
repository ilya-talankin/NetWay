#include "mythread.h"
#include "qtextedit.h"
#include "myrelay.h"

MyRelay::MyRelay(const QString& arg1, const QString& arg2, QTextEdit* log, QTcpServer *parent)
    : QTcpServer{parent}, log(log)
{
    setArgs(arg1, arg2);
    startServer();
}

void MyRelay::setArgs(const QString& arg1, const QString& arg2)
{
    hostSocket = arg1;
    hostRemoteSocket = arg2;
}

void MyRelay::startServer()
{
    tcpSocket = new QTcpSocket(this);
    //tcpRemoteSocket = new QTcpSocket();
    QHostAddress ip = QHostAddress(hostSocket.split(':').first());
    int port = hostSocket.split(':').last().toInt();
    if (!this->listen(ip, port)){
        log->append("Could not start server");
        return;
    } else {
        log->append("Listening to socket " + ip.toString() + ":" + QString::number(port));
    }
    dataIn.setDevice(tcpSocket);
    dataIn.setVersion(QDataStream::Qt_6_5);
    QHostAddress ipRemote = QHostAddress(hostRemoteSocket.split(':').first());
    int portRemote = hostRemoteSocket.split(':').last().toInt();
    tcpSocket->connectToHost(ipRemote, portRemote);
    connect(tcpSocket, &QIODevice::readyRead, this, &MyRelay::readData);
    //sendToServer(tcpRemoteSocket, QString("1"));
}

void MyRelay::incomingConnection(qintptr socketDescriptor)
{
    //qDebug()<<"Incoming conn";
    log->append(QString::number(socketDescriptor) + " Connecting...");
    MyThread *thread = new MyThread(socketDescriptor, log, this);
    connect(thread, &MyThread::finished, thread, &MyThread::deleteLater);
    connect(thread, &MyThread::finished, this, [socketDescriptor, this](){sendMsg(QString::number(socketDescriptor) + " disconnected");});
    connect(thread, &MyThread::newMsg, this, &MyRelay::sendMsg);
    thread->start();
}

void MyRelay::sendMsg(const QString& data){
    sendToServer(/*tcpRemoteSocket, */data);
}

void MyRelay::sendToServer(/*QTcpSocket* serverSocket, */const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    //out << QTime::currentTime() << str;

    //out.device()->seek(0);
    //out << quint16(arrBlock.size() - sizeof(quint16));
    out << str;
    //qDebug()<<"Sending";
    tcpSocket->write(arrBlock);
}

void MyRelay::requestNewData()
{
    //QHostAddress ip = QHostAddress(hostSocket.split(':').first());
    //int port = hostSocket.split(':').last().toInt();
    //tcpSocket->connectToHost(ip, port);
    //qDebug()<<"Trying to connect";
    //QTcpSocket server;
    //server.bind(ip, port);
    sendToServer(/*tcpSocket, */"raw");
    //qDebug()<<"Sent";
    //stopButton->setEnabled(true);
}
/*
void MyRelay::abortConnection()
{
    stopButton->setEnabled(false);
    tcpSocket->abort();
    requestButton->setEnabled(true);
}*/

void MyRelay::readData()
{
    dataIn.startTransaction();

    QString data;
    dataIn >> data;

    if (!dataIn.commitTransaction())
        return;
    log->append(data);


    //currentFortune = nextFortune;
    //statusLabel->setText(currentFortune);

    //requestButton->setEnabled(false);
}

