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

protected:
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
};

#endif // MAINWINDOW_H
