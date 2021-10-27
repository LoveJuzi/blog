#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

#include "triangle.h"
#include "square.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLFunctions_3_3_Core* _glcore;

    std::unique_ptr<Triangle> _triangle;
    std::unique_ptr<Square> _square;
};
#endif // MAINWINDOW_H
