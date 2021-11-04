#ifndef TWINDOW_H
#define TWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class TWindow : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core {
public:
    TWindow();
    ~TWindow() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

private:
    GLuint VAO;           // VAO 对象
    GLuint VBO;           // VBO 对象
    GLuint EBO;           // EBO 对象
    GLuint shaderProgram; // 着色器程序
};

#endif // TWINDOW_H
