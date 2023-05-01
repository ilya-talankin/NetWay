
#ifndef WINDOW_H
#define WINDOW_H


#include <QObject>
#include <QtWidgets/QtWidgets>
#include <QWidget>
#include "myserver.h"
#include <QSharedPointer>


class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(const QStringList& arguments, QWidget *parent = nullptr);

signals:

private:
    QTextEdit* log;
    MyServer* server;
    QPushButton *stopButton = nullptr;
};

#endif // WINDOW_H
