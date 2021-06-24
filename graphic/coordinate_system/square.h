#ifndef SQUARE_H
#define SQUARE_H

#include <memory>

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class Square
{
public:
    Square(QOpenGLFunctions_3_3_Core* glcore);

    int init();
    int paint();

    void setModel(const glm::mat4& mat);
    void setView(const glm::mat4& mat);
    void setProjection(const glm::mat4& mat);

private:
    QOpenGLFunctions_3_3_Core* _glcore;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture1;
    GLuint texture2;

    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;

    std::unique_ptr<Shader> _shader;
};

#endif // SQUARE_H
