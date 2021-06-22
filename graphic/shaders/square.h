#ifndef SQUARE_H
#define SQUARE_H

#include <QOpenGLFunctions_3_3_Core>

class Square
{
public:
    Square(QOpenGLFunctions_3_3_Core* glcore);

    int init();
    int paint();

private:
    QOpenGLFunctions_3_3_Core* _glcore;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint shaderProgram;
};

#endif // SQUARE_H
