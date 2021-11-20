#ifndef TWINDOW_H
#define TWINDOW_H

#include <QtOpenGL/QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

#include "utils/Shader.h"
#include "utils/Camera.h"

// twindow.cpp
class TWindow : public QOpenGLWindow {
public:
    TWindow();
    ~TWindow() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;

private:
    GLuint VAO;           // VAO 对象
    GLuint VBO;           // VBO 对象
    GLuint EBO;           // EBO 对象
    GLuint VBO2;           // VBO 对象
    GLuint EBO2;           // EBO 对象

    Shader shader;        // 着色器
    Camera camera;        // 摄像机

    GLfloat deltaTime;    // 移动速度
    QPoint  center;       // 鼠标中心点
};

#endif // TWINDOW_H
