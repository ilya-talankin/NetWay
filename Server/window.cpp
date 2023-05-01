#include "window.h"

Window::Window(const QStringList& arguments, QWidget *parent)
    : QWidget{parent}, log(new QTextEdit), stopButton(new QPushButton("Start"))
{
    log->setReadOnly(true);
    QVBoxLayout* myBoxLayout = new QVBoxLayout;
    myBoxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    myBoxLayout->addWidget(log);
    myBoxLayout->addWidget(stopButton);
    setLayout(myBoxLayout);
    server = new MyServer(arguments, log);
    //connect(stopButton, &QPushButton::clicked, server, &MyServer::startSending);
}

