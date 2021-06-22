#include "mainwindow.h"

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
}


void MainWindow::initializeGL()
{
    resize(QSize(800, 800));

    _glcore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

    _triangle.reset(new Triangle(_glcore));
    _triangle->init();

    _square.reset(new Square(_glcore));
    _square->init();
}

void MainWindow::resizeGL(int w, int h)
{
    _glcore->glViewport(0, 0, w, h);
}

void MainWindow::paintGL()
{
    // clear background
    _glcore->glClearColor(0.2f, 0.3f, 0.3f, 1.0);
    _glcore->glClear(GL_COLOR_BUFFER_BIT);

    _square->paint();
    _triangle->paint();
}
