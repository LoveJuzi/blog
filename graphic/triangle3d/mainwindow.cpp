#include "mainwindow.h"

#include <iostream>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

MainWindow::MainWindow()
{
    _vertices = new std::vector<GLfloat> {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete _vertices;
}

void MainWindow::initializeGL()
{
    std::cout << "======>initializeGL" << std::endl;
    if (!initializeOpenGLFunctions())
    {
        std::cerr << "initializeOpenGLFunctions failed!" << std::endl;
    }

    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(_vertexShader);

    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(_fragmentShader);

    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, _vertexShader);
    glAttachShader(_shaderProgram, _fragmentShader);
    glLinkProgram(_shaderProgram);
    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices->size(),
                          &(*_vertices)[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
}

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MainWindow::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(_shaderProgram);
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

