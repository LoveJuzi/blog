#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

class MainWindow : public QOpenGLWindow, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

protected:
    std::vector<GLfloat>* _vertices;

    GLuint _VBO;
    GLuint _VAO;
    GLuint _vertexShader;
    GLuint _fragmentShader;
    GLuint _shaderProgram;
};
#endif // MAINWINDOW_H
