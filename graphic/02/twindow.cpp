#include "twindow.h"

#include <iostream>

#include "utils/utDefer.h"
#include "utils/OpenGLSingleton.h"

static GLfloat cubeVerices[] = {
     0.5f,  0.5f,  0.5f,   // 前右上角  0
    -0.5f,  0.5f,  0.5f,   // 前左上角  1
    -0.5f, -0.5f,  0.5f,   // 前左下角  2
     0.5f, -0.5f,  0.5f,   // 前右下角  3
     0.5f,  0.5f, -0.5f,   // 后右上角  4
    -0.5f,  0.5f, -0.5f,   // 后左上角  5
    -0.5f, -0.5f, -0.5f,   // 后左下角  6
     0.5f, -0.5f, -0.5f,   // 后右下角  7
};

static GLuint cubeIndices[] = {
    0, 1, 2,  // 前上三角形
    2, 3, 0,  // 前下三角形
    5, 4, 7,  // 后上三角形
    7, 6, 5,  // 后下三角形
    4, 5, 1,  // 上上三角形
    1, 0, 4,  // 上下三角形
    6, 7, 3,  // 下上三角形
    3, 2, 6,  // 下下三角形
    7, 4, 0,  // 右上三角形
    0, 3, 7,  // 右下三角形
    5, 6, 2,  // 左上三角形
    2, 1, 5,  // 左下三角形
};

//static float triangleVertices[] = {
//     0.0f,  0.5f, 0.0f,  // 顶点
//    -0.5f, -0.5f, 0.0f,  // 左下角
//     0.5f, -0.5f, 0.0f,  // 右下角
//};
//
//static GLuint triangleIndices[] = {
//    0, 1, 2,  // 第一个三角形
//};


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
    OpenGLInstance->glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerices), cubeVerices, GL_STATIC_DRAW);

    // bind EBO
    OpenGLInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // EBO buffer data
    OpenGLInstance->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // set vertex attribute pointer
    OpenGLInstance->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // enable vertex atrribute array
    OpenGLInstance->glEnableVertexAttribArray(0);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // 3. init window size
    resize(QSize(800, 800));
}

void TWindow::resizeGL(int w, int h) {
    OpenGLInstance->glViewport(0, 0, w, h);
}

void TWindow::paintGL() {
    // clear background
    OpenGLInstance->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    OpenGLInstance->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use shader program
    shader.use();

    // bind vertex array object
    OpenGLInstance->glBindVertexArray(VAO);

    // draw
    // OpenGLInstance->glDrawElements(GL_TRIANGLES, sizeof(cubeIndices)/sizeof(GLuint), GL_UNSIGNED_INT, (GLvoid*)0);
    OpenGLInstance->glDrawElements(GL_LINE_STRIP, sizeof(cubeIndices)/sizeof(GLuint), GL_UNSIGNED_INT, (GLvoid*)0);

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
