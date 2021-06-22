#include "square.h"

Square::Square(QOpenGLFunctions_3_3_Core* glcore)
    : _glcore(glcore)
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

static const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n\0";

static const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

int Square::init()
{
    // shader
    auto vertexShader = _glcore->glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShader = _glcore->glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = _glcore->glCreateProgram();

    _glcore->glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    _glcore->glCompileShader(vertexShader);

    _glcore->glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    _glcore->glCompileShader(fragmentShader);

    _glcore->glAttachShader(shaderProgram, vertexShader);
    _glcore->glAttachShader(shaderProgram, fragmentShader);
    _glcore->glLinkProgram(shaderProgram);

    _glcore->glDeleteShader(vertexShader);
    _glcore->glDeleteShader(fragmentShader);

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
    _glcore->glUseProgram(shaderProgram);

    _glcore->glBindVertexArray(VAO);

    _glcore->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);

    _glcore->glBindVertexArray(0);

    return 0;
}
