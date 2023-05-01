#include "window.h"
#include <QtWidgets/QtWidgets>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window window(a.arguments());
    window.show();
    return a.exec();
}
