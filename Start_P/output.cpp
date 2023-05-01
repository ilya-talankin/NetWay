#include "output.h"

Output::Output()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Output::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, QTcpSocket::deleteLater);

    socket->connectToHost("127.0.0.1", port);

    std::cout << "Start Output messenger" << std::endl;
    std::cout << "Enter string for send: " << std::endl;

    QTextStream s(stdin);
    QString str = s.readLine();

    SendToRetranslator(str);
}

void Output::SendToRetranslator(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
}

void Output::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }

            //quint16 r_weight;
            in >> status_sender;
            //in >> r_weight;
            nextBlockSize = 0;
            /*if(best_weight > r_weight)
            {
                best_weight = r_weight;
            }*/
            if (status_sender == true)
            {

            qDebug() << "Status sender: " << status_sender;
            status_sender = false;

            std::cout << "Enter string for send: " << std::endl;

            QTextStream s(stdin);
            QString str = s.readLine();

            SendToRetranslator(str);
            }
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }

}
