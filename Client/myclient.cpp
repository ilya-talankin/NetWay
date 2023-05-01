#include <QtNetwork>
#include "myclient.h"
#include "qtextedit.h"

MyClient::MyClient(const QStringList& arguments, QWidget *parent)
    : QWidget{parent},
    log(new QTextEdit),
    tcpSocket(new QTcpSocket(this)),
    requestButton(new QPushButton("Request")),
    stopButton(new QPushButton("Stop")),
    showButton(new QPushButton("Show")),
    speed(new QLabel("0 packets/sec"))
{
    log->setReadOnly(true);
    QVBoxLayout* myBoxLayout = new QVBoxLayout;
    myBoxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(new QLabel("Current speed: "));
    horizontalLayout->addWidget(speed);
    myBoxLayout->addLayout(horizontalLayout);
    myBoxLayout->addWidget(log);
    QHBoxLayout* horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->addWidget(requestButton);
    horizontalLayout2->addWidget(stopButton);
    horizontalLayout2->addWidget(showButton);
    myBoxLayout->addLayout(horizontalLayout2);
    setLayout(myBoxLayout);
    setArgs(arguments);

    dataIn.setDevice(tcpSocket);
    dataIn.setVersion(QDataStream::Qt_6_5);

    connect(requestButton, &QAbstractButton::clicked,
            this, &MyClient::requestNewData);

    connect(stopButton, &QAbstractButton::clicked,
            this, &MyClient::abortConnection);
    //connect(showButton, &QAbstractButton::clicked,
     //       this, &MyClient::showResults);
    requestButton->setEnabled(true);
    stopButton->setEnabled(false);
    showButton->setEnabled(false);
    /*for (const auto& serverSocket : hostSockets) {
        QTcpSocket* mySocket = new QTcpSocket(this);
        mySocket->connectToHost(serverSocket.peerAddress(), serverSocket.peerPort());
        connect(mySocket, &QTcpSocket::connected, this, &MyClient::hi);
        connect(mySocket, &QIODevice::readyRead, this, &MyClient::readData);
        mySockets.push_back(std::move(mySocket));
    }*/
}

void MyClient::setArgs(const QStringList& arguments)
{
    for (int count = 1; count < arguments.count(); ++count) {
        if (arguments[count].compare("-l") == 0) {
            amount = arguments[++count].toInt();
        } else
        if (arguments[count].compare("-cn") == 0) {
            //amount = arguments[count].toInt();
            QHostAddress ip = QHostAddress(arguments[++count].split(':').first());
            int port = arguments[count].split(':').last().toInt();
            //connectTo.bind(ip, port);
            tcpSocket->connectToHost(ip, port);
            //connect(tcpSocket, &QTcpSocket::connected, this, &MyClient::requestNewData);
            connect(tcpSocket, &QIODevice::readyRead, this, &MyClient::readData);
        }/*
        else if (arguments[count++].compare("-ip") == 0) {
            while (arguments[count].compare("-cn") != 0 &&
                   arguments[count].compare("-l") != 0 &&
                   count < arguments.count()){
                QHostAddress ip = QHostAddress(arguments[count].split(':').first());
                int port = arguments[count++].split(':').last().toInt();
                QTcpSocket socket;
                socket.bind(ip, port);
                //hostSockets.push_back(socket);
            }
            continue;
        }*/
    }
}

void MyClient::sendToServer(QTcpSocket* serverSocket, const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    //out << QTime::currentTime() << str;

    //out.device()->seek(0);
    //out << quint16(arrBlock.size() - sizeof(quint16));
    out << str/* << "end" << "15.178.19"*/;
    //arrBlock<<qint64{0x24a23d89};
    //QByteArray as("e");
    //qDebug()<<arrBlock.contains(as);
    serverSocket->write(arrBlock);
}

void MyClient::requestNewData()
{

    //qDebug()<<"Trying to connect";
    //QTcpSocket server;
    //server.bind(ip, port);
    sendToServer(tcpSocket, "raw");
    //log->append("raw");
    //qDebug()<<"Sent";
    stopButton->setEnabled(true);
    //requestButton->setEnabled(false);
}

void MyClient::abortConnection()
{
    stopButton->setEnabled(false);
    tcpSocket->abort();
    requestButton->setEnabled(true);
}

void MyClient::readData()
{
    //QTcpSocket* senderSocket = static_cast<QTcpSocket*>(sender());
    //dataIn.setDevice(tcpSocket);
    //dataIn.setVersion(QDataStream::Qt_6_5);
    dataIn.startTransaction();

    QString data;
    dataIn >> data;

    if (!dataIn.commitTransaction())
        return;
    log->append(data);
    /*if (nextFortune == currentFortune) {
        QTimer::singleShot(0, this, &Client::requestNewFortune);
        return;
    }*/

    //currentFortune = nextFortune;
    //statusLabel->setText(currentFortune);

    //requestButton->setEnabled(true);
}

