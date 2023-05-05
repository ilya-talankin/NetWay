#ifndef RETRANSLATOR_H
#define RETRANSLATOR_H

#include "../Server/server.h"
#include "../Client/client.h"

class Retranslator : public QObject
{
    Q_OBJECT

public:
    explicit Retranslator(quint16 id, quint64 delay, quint16 serverPort, const QVector<quint16>& portsVec, QObject *parent = nullptr);

private slots:
    void redirect(const QString& message);
private:
    Server* server = nullptr;
    Client* client = nullptr;
    QDataStream in;
};

#endif // RETRANSLATOR_H
