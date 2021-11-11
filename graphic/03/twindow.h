#ifndef TWINDOW_H
#define TWINDOW_H

#include <QOpenGLWindow>
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
    Camera  camera;        // 摄像机
    GLfloat deltaTime;     // 移动速度
    QPoint  center;        // 鼠标中心点
    bool    initCursor;    // 鼠标位置初始化
};

#endif // TWINDOW_H
