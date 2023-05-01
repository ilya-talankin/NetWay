#include "input.h"

Input::Input()
{
    if(this->listen(QHostAddress::Any, port))
    {
        qDebug() << "start listen ...";
    }
    else
    {
        qDebug() << "error";
    }
}


void Input::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Input::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;
}

void Input::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "read...";

        for(;;)
        {
            if(nextBlockSize == 0)
            {
                //qDebug() << "nextBlockSize = 0";
                if(socket->bytesAvailable() < 2)
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
                //qDebug() << "nextBlockSize = " << nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data not full, break";
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            qDebug() << str;
            status_sender = true;
            SendToClient(status_sender);
            break;
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }
}


void Input::SendToClient(bool status)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << status;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));

    socket->write(Data);
    status_sender = false;
}
