#include <QGuiApplication>

#include "trianglewindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    TriangleWindow w;
    w.show();

    return a.exec();
}
