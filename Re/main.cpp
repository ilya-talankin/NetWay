#include <QCoreApplication>
#include "retranslator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    if (argc < 4) {
        qDebug() << "Invalid arguments";
        return 0;
    }
    quint16 id, serverPort;
    //Retranslator ID
    id = std::stoi(argv[1]);
    //Retranslator port
    serverPort = std::stoi(argv[2]);
    //ports connect to (for client side)
    QVector<quint16> portsVec;
    try {
        for (int i = 3; i < argc; ++i){
            portsVec.push_back(std::stoi(argv[i]));
        }
    }
    catch(std::exception& ex) {
        qDebug() << ex.what();
    }

    Retranslator retranslator(id, serverPort, portsVec);
    return app.exec();
}
