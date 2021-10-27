#ifndef SQUARELIGHTSOURCE_H
#define SQUARELIGHTSOURCE_H

#include <QOpenGLFunctions_3_3_Core>

#include "globject.h"

class SquareLightSource : public GLObject
{
public:
    SquareLightSource(QOpenGLFunctions_3_3_Core* glcore);

    int init() override;
    int paint() override;

private:
    GLuint VAO;
    GLuint VBO;
};

#endif // SQUARELIGHTSOURCE_H
