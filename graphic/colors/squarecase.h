#ifndef SQUARECASE_H
#define SQUARECASE_H

#include <QOpenGLFunctions_3_3_Core>

#include "globject.h"

class SquareCase : public GLObject
{
public:
    SquareCase(QOpenGLFunctions_3_3_Core* glcore);

    int init() override;
    int paint() override;

private:

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture1;
    GLuint texture2;
};

#endif // SQUARECASE_H
