#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "openglwindow.h"

class MainWindow : public OpenGLWindow
{
public:
    MainWindow();

    void initialize() override;
    void render() override;
};

#endif // MAINWINDOW_H
