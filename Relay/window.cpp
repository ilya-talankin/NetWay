#include "window.h"

Window::Window(const QString& arg1, const QString& arg2, QWidget *parent)
    : QWidget{parent}, log(new QTextEdit), stopButton(new QPushButton("Stop"))
{
    log->setReadOnly(true);
    QVBoxLayout* myBoxLayout = new QVBoxLayout;
    myBoxLayout->addWidget(new QLabel("<H1>Relay</H1>"));
    myBoxLayout->addWidget(log);
    myBoxLayout->addWidget(stopButton);
    setLayout(myBoxLayout);
    relay = new MyRelay(arg1, arg2, log);
    connect(stopButton, &QPushButton::clicked, relay, &MyRelay::requestNewData);
}

