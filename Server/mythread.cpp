#include "mythread.h"

MyThread::MyThread(qintptr ID, QTextEdit* log, QObject *parent) :
    QThread(parent), socketDescriptor(ID), m_log(log)
{
}

void MyThread::run()
{
    m_log->append("New thread started");
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor)){
        emit error(socket->error());
        return;
    }
    connect(socket, &QTcpSocket::readyRead, this, &MyThread::readClient, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &MyThread::disconnected);
    m_log->append(QString::number(socketDescriptor) + " Client connected");
    //connect(this, &MyThread::newMsg, this, &MyThread::sendToClient);
    //dataIn.setDevice(socket);
    //dataIn.setVersion(QDataStream::Qt_6_5);
    //sendToClient("Ok");
    exec();
}

void MyThread::readClient()
{
    //dataIn.startTransaction();
    QByteArray Data = socket->readAll();

    m_log->append(QString::number(socketDescriptor) + " Data in: " + Data);
    //emit newMsg(Data);
    //sendToClient("Ok");
}

void MyThread::hello(){
    sendToClient(QString("Hello"));
}

void MyThread::sendToClient(const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str;
    socket->write(arrBlock);
}

void MyThread::disconnected()
{
    m_log->append(QString::number(socketDescriptor) + " Disconnected");
    socket->deleteLater();
    exit(0);
}
