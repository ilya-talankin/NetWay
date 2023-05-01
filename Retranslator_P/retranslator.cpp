#include "retranslator.h"

Retranslator::Retranslator()
{
    if(this->listen(QHostAddress::Any, port_in))
    {
        qDebug() << "start listen information from Output...";
    }
    else
    {
        qDebug() << "error";
    }

    socket_out = new QTcpSocket(this);
    //connect(socket_out, &QTcpSocket::readyRead, this, &Retranslator::slotReadyReadOut);
    connect(socket_out, &QTcpSocket::disconnected, socket_out, QTcpSocket::deleteLater);

    //socket_out->connectToHost("127.0.0.1", port_out);

    //socket_in->connectToHost("127.0.0.1", port_in);

    socket_out->connectToHost("127.0.0.1", port_out);

}

void Retranslator::incomingConnection(qintptr socketDescriptor)
{
    socket_in = new QTcpSocket;
    socket_in->setSocketDescriptor(socketDescriptor);
    connect(socket_in, &QTcpSocket::readyRead, this, &Retranslator::slotReadyReadIn);
    connect(socket_in, &QTcpSocket::disconnected, socket_in, QTcpSocket::deleteLater);

    Sockets.push_back(socket_in);
    qDebug() << "client connected" << socketDescriptor;
}

void Retranslator::slotReadyReadIn()
{
    socket_in = (QTcpSocket*)sender();
    QDataStream in(socket_in);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "read...";

        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(socket_in->bytesAvailable() < 2)
                {
                    qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket_in->bytesAvailable() < nextBlockSize)
            {
                qDebug() << "Data not full, break";
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            qDebug() << str;
            status_sender = true;
            SendToClientData(str);
            SendToClientStatus(status_sender);
            break;
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }
}

void Retranslator::slotReadyReadOut()
{

}

void Retranslator::SendToClientData(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));

    socket_out->write(Data);

    qDebug() << "Resend Data";
}

void Retranslator::SendToClientStatus(bool status)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << status;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));

    socket_in->write(Data);

    status_sender = false;
}
