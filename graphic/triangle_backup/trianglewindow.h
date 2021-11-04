#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

class TriangleWindow : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core
{
public:
    TriangleWindow();
    ~TriangleWindow() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    GLuint VAO;           // VAO 对象
    GLuint VBO;           // VBO 对象
    GLuint EBO;           // EBO 对象
    GLuint shaderProgram; // 着色器程序
};

#endif // TRIANGLEWINDOW_H
