#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLFunctions_3_3_Core>

class Triangle
{
public:
    Triangle(QOpenGLFunctions_3_3_Core* glcore);

    int init();
    int paint();

private:
    QOpenGLFunctions_3_3_Core* _glcore;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint shaderProgram;
};

#endif // TRIANGLE_H
