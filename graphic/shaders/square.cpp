#include "square.h"

Square::Square(QOpenGLFunctions_3_3_Core* glcore)
    : _glcore(glcore), _shader(new Shader(glcore))
{

}

static float vertices[] = {
     0.5f,  0.5f, 0.0f,  // 右上角
     0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f,  // 左下角
    -0.5f,  0.5f, 0.0f   // 左上角
};

static GLuint indices[] = {
    0, 1, 3,  // 第一个三角形
    1, 2, 3   // 第二个三角形
};

int Square::init()
{
    // shader
    _shader->build("square.vs.glsl", "square.fs.glsl");

    // vertex data
    _glcore->glGenVertexArrays(1, &VAO);
    _glcore->glGenBuffers(1, &VBO);
    _glcore->glGenBuffers(1, &EBO);

    _glcore->glBindVertexArray(VAO);

    _glcore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    _glcore->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    _glcore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
    _glcore->glEnableVertexAttribArray(0);

    _glcore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    _glcore->glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(indices), indices, GL_STATIC_DRAW);

    return 0;
}

int Square::paint()
{
    _shader->use();

    _glcore->glBindVertexArray(VAO);

    _glcore->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);

    _glcore->glBindVertexArray(0);

    return 0;
}
