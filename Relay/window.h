
#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>
#include <QtWidgets/QtWidgets>
#include <QWidget>
#include "myrelay.h"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(const QString& arg1, const QString& arg2, QWidget *parent = nullptr);

signals:
private:
    QTextEdit* log;
    MyRelay* relay;
    QPushButton *stopButton = nullptr;
};

#endif // WINDOW_H
