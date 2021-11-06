#include "twindow.h"

#include <iostream>

#include "utils/utDefer.h"
#include "utils/OpenGLSingleton.h"

//static GLfloat vertices[] = {
//     0.5f,  0.5f, 0.0f,  // 右上角
//     0.5f, -0.5f, 0.0f,  // 右下角
//    -0.5f, -0.5f, 0.0f,  // 左下角
//    -0.5f,  0.5f, 0.0f   // 左上角
//};
//
//static GLuint indices[] = {
//    0, 1, 3,  // 第一个三角形
//    1, 2, 3   // 第二个三角形
//};

static float triangleVertices[] = {
     0.0f,  0.5f, 0.0f,  // 顶点
    -0.5f, -0.5f, 0.0f,  // 左下角
     0.5f, -0.5f, 0.0f,  // 右下角
};

static GLuint triangleIndices[] = {
    0, 1, 2,  // 第一个三角形
};


TWindow::TWindow() {
}

TWindow::~TWindow() {
}

void TWindow::initializeGL() {
    OpenGLInstanceInit(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>());

    if (OpenGLInstance == NULL) {
        std::cerr << "OpenGL init error!" << std::endl;
        close();
        return ;
    }

    // 1. init shader
    if (!shader.init("simpleVertexShader.glsl", "simpleFragmentShader.glsl")) {
        close();
        return;
    }
    
    // 2. deal vertex data
    // create VAO
    OpenGLInstance->glGenVertexArrays(1, &VAO);

    // create VBO
    OpenGLInstance->glGenBuffers(1, &VBO);

    // create EBO
    OpenGLInstance->glGenBuffers(1, &EBO);

    // bind VAO
    OpenGLInstance->glBindVertexArray(VAO);

    // bind VBO
    OpenGLInstance->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // VAO buffer data
    OpenGLInstance->glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // bind EBO
    OpenGLInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // EBO buffer data
    OpenGLInstance->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    // set vertex attribute pointer
    OpenGLInstance->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

    // 3. init window size
    resize(QSize(800, 800));
}

void TWindow::resizeGL(int w, int h) {
    OpenGLInstance->glViewport(0, 0, w, h);
}

void TWindow::paintGL() {
    // clear background
    OpenGLInstance->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    OpenGLInstance->glClear(GL_COLOR_BUFFER_BIT);

    // use shader program
    shader.use();

    // bind vertex array object
    OpenGLInstance->glBindVertexArray(VAO);

    // enable vertex atrribute array
    OpenGLInstance->glEnableVertexAttribArray(0);

    // draw
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    OpenGLInstance->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

    // release VAO
    OpenGLInstance->glBindVertexArray(0);
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
