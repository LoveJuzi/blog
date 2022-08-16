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

    const glm::vec3& getLightColor() const;

private:
    GLuint VAO;
    GLuint VBO;

    glm::vec3 _lightColor;
};

#endif // SQUARELIGHTSOURCE_H
