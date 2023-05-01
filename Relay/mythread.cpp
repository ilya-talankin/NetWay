
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
    //emit newMsg("Data");
    exec();
}

void MyThread::readClient()
{
    //QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(sender());
    //QDataStream in(socket);
    //in.setVersion(QDataStream::Qt_6_5);
    //for (;;) {
    /*if (!nextBlockSize) {
            if (socket->bytesAvailable() < static_cast<int>(sizeof(quint16))) {
                break;
            }
            nextBlockSize = socket->bytesAvailable();
        }*/
    //qDebug()<<"Got";
    /*if (socket->bytesAvailable() < nextBlockSize) {
            break;
        }*/
    //QTime time;
    //QString str;
    //in >> time >> str;

    //QString strMessage = time.toString() + " " + "Client has sent - " + str;
    //m_log->append(strMessage);

    //nextBlockSize = 0;
    //if (str == "raw")
    //sendToClient(socket, QString("Server Response: Received "/* + str + "\""*/));
    //}
    QByteArray Data = socket->readAll();
    //m_log->append(QString::number(socketDescriptor) + " Data in: " + Data);

    //socket->write(Data);
    //QByteArray Data = socket->readAll();
    //QDataStream dataIn(Data);
    //dataIn.startTransaction();
    //QString wqe;
    //dataIn >> wqe;
    //QString data;
    //QByteArray data = Data;
    //if (wqe.compare("1") == 0){
        //m_log->append(QString(Data));
        emit newMsg(Data);
    //}
    //m_log->append(QString(Data)+"45");
    //m_log->append(QString::number(socketDescriptor) + " Data in: " + Data);
    //sendToClient("Ok");
    //socket->write(Data);
}

void MyThread::sendToClient(/*QTcpSocket *clientSocket, */const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out /*<< quint16(0) << QTime::currentTime()*/ << str;

    //out.device()->seek(0);
    //out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}

void MyThread::disconnected()
{
    m_log->append(QString::number(socketDescriptor) + " Disconnected");
    socket->deleteLater();
    exit(0);
}

