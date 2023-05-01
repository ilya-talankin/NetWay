
#include "window.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window window("127.0.0.1:9001", "127.0.0.1:9000");
    window.show();
    return a.exec();
}
