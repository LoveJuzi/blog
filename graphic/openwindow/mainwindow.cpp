#include "mainwindow.h"

#include <iostream>

MainWindow::MainWindow()
{
    resize(QSize(800, 600));
}

MainWindow::~MainWindow()
{

}
GLuint VBO, VAO;
const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n\0";
const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

unsigned int shaderProgram;

void MainWindow::initializeGL()
{
    if (!initializeOpenGLFunctions())
    {
        std::cout << "error" << std::endl;
    }

    unsigned int vertexShader;
    unsigned int fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, 800, 600);
}

void MainWindow::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}

void MainWindow::keyPressEvent(QKeyEvent *)
{

}

void MainWindow::keyReleaseEvent(QKeyEvent *)
{

}

void MainWindow::mousePressEvent(QMouseEvent *)
{

}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *)
{

}

void MainWindow::mouseMoveEvent(QMouseEvent *)
{

}


