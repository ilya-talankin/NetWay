#include <QCoreApplication>
#include <server.h>
#include <string>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (argc < 3) {
        qDebug() << "Invalid argument";
        qDebug() << argc;
        return 0;
    }
    quint16 port = std::stoi(argv[1]);
    quint16 id = std::stoi(argv[2]);
    Server server(port, id);
    return a.exec();
}
