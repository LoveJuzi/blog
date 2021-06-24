#include "square.h"

#include <iostream>

#include "stb_image.h"

Square::Square(QOpenGLFunctions_3_3_Core* glcore)
    : _glcore(glcore), _shader(new Shader(glcore))
{

}

static GLfloat vertices[] = {
    // 位置               // 颜色             // 纹理
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,  0.8f, 0.5f, 0.2f,   0.0f, 1.0f,
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
    _glcore->glGenTextures(1, &texture);

    _glcore->glBindVertexArray(VAO);

    _glcore->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    _glcore->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    _glcore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
    _glcore->glEnableVertexAttribArray(0);
    _glcore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    _glcore->glEnableVertexAttribArray(1);
    _glcore->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (GLvoid*)(6 * sizeof(float)));
    _glcore->glEnableVertexAttribArray(2);

    _glcore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    _glcore->glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(indices), indices, GL_STATIC_DRAW);

    _glcore->glBindTexture(GL_TEXTURE_2D, texture);
    _glcore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    _glcore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    _glcore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    _glcore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        _glcore->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        _glcore->glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }

    return 0;
}

int Square::paint()
{
    _shader->use();

    _glcore->glBindTexture(GL_TEXTURE_2D, texture);

    _glcore->glBindVertexArray(VAO);

    _glcore->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);

    _glcore->glBindVertexArray(0);

    return 0;
}
