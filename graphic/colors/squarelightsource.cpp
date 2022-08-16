#include "squarelightsource.h"

#include <glm/gtc/type_ptr.hpp>

SquareLightSource::SquareLightSource(QOpenGLFunctions_3_3_Core* glcore)
    : GLObject(glcore)
{

    _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

static GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int SquareLightSource::init()
{
    _glcore->glGenVertexArrays(1, &VAO);
    _glcore->glGenBuffers(1, &VBO);

    _glcore->glBindVertexArray(VAO);
    _glcore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    _glcore->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    _glcore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
    _glcore->glEnableVertexAttribArray(0);

    _shader->build("squarelightsource.vs.glsl", "squarelightsource.fs.glsl");

    _shader->use();
    _shader->setVec3("lightColor", _lightColor);

    return 0;
}

int SquareLightSource::paint()
{
    glm::mat4 zoom = glm::mat4(1.0f);
    zoom = glm::scale(zoom, glm::vec3(0.2f, 0.2f, 0.2f));

    _shader->use();

    _shader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(_model));
    _shader->setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(_view));
    _shader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(_projection));
    _shader->setMatrix4fv("zoom", 1, GL_FALSE, glm::value_ptr(zoom));

    _glcore->glBindVertexArray(VAO);

    _glcore->glDrawArrays(GL_TRIANGLES, 0, 36);

    _glcore->glBindVertexArray(0);

    return 0;
}

const glm::vec3& SquareLightSource::getLightColor() const
{
    return _lightColor;
}

