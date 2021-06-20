#include "mainwindow.h"

MainWindow::MainWindow()
{
    resize(QSize(800, 600));
}

void MainWindow::initialize()
{
}

void MainWindow::render()
{
    glViewport(0, 0, 800, 600);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainWindow::keyPressEvent(QKeyEvent *)
{

}

void MainWindow::keyReleaseEvent(QKeyEvent *)
{

}

void MainWindow::mousePressEvent(QMouseEvent *)
{

}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *)
{

}

void MainWindow::mouseMoveEvent(QMouseEvent *)
{

}


