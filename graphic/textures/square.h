#ifndef SQUARE_H
#define SQUARE_H

#include <memory>

#include <QOpenGLFunctions_3_3_Core>

#include "shader.h"

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
    GLuint texture1;
    GLuint texture2;

    std::unique_ptr<Shader> _shader;
};

#endif // SQUARE_H
