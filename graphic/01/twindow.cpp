#include "twindow.h"

#include <iostream>

#include <utils/utDefer.h>

TWindow::TWindow() {
}

TWindow::~TWindow() {
}

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // 右上角
     0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f,  // 左下角
    -0.5f,  0.5f, 0.0f   // 左上角
};

GLuint indices[] = {
    0, 1, 3,  // 第一个三角形
    1, 2, 3   // 第二个三角形
};

float triangleVertices[] = {
     0.0f,  0.5f, 0.0f,  // 顶点
    -0.5f, -0.5f, 0.0f,  // 左下角
     0.5f, -0.5f, 0.0f,  // 右下角
};

GLuint triangleIndices[] = {
    0, 1, 2,  // 第一个三角形
};

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

void TWindow::initializeGL() {
    if (!initializeOpenGLFunctions()) {
        std::cerr << "initializeOpenGLFunctions failed!" << std::endl;
    }

    // 1. deal shader
    // create vertex shader
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // delete vertex shader
    utDefer(glDeleteShader(vertexShader));

    // create fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // delete fragment shader
    utDefer(glDeleteShader(fragmentShader));

    // create sharder
    shaderProgram = glCreateProgram();

    // set vertex shader source
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

    // compile vertex shader
    glCompileShader(vertexShader);

    // set fragment shader source
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

    // compile fragment shader
    glCompileShader(fragmentShader);

    // attach vertex shader
    glAttachShader(shaderProgram, vertexShader);

    // attach fragment shader
    glAttachShader(shaderProgram, fragmentShader);

    // link shaders
    glLinkProgram(shaderProgram);

    // 2. deal vertex data
    // create VAO
    glGenVertexArrays(1, &VAO);

    // create VBO
    glGenBuffers(1, &VBO);

    // create EBO
    glGenBuffers(1, &EBO);

    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // VAO buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // EBO buffer data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    // set vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    // 3. init window size
    resize(QSize(600, 600));
}

void TWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void TWindow::paintGL() {
    // clear background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // use shader program
    glUseProgram(shaderProgram);

    // bind vertex array object
    glBindVertexArray(VAO);

    // enable vertex atrribute array
    glEnableVertexAttribArray(0);

    // draw
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

    // release VAO
    glBindVertexArray(0);
}

void TWindow::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        close();
        return;
    }
}

void TWindow::mouseMoveEvent(QMouseEvent *e) {
    Q_UNUSED(e);
}
