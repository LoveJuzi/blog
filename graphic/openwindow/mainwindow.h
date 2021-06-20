#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core
{
public:
    MainWindow();
    ~MainWindow() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

protected:
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
};

#endif // MAINWINDOW_H
