#include <QGuiApplication>

#include "twindow.h"

int main(int argc, char *argv[]) {
    QGuiApplication a(argc, argv);

    TWindow w;
    w.show();

    return a.exec();
}
