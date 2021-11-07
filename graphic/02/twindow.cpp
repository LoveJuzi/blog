#include "twindow.h"

#include <iostream>

#include "utils/utDefer.h"
#include "utils/OpenGLSingleton.h"

static GLfloat cubeVerices[] = {
     // position          // color
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  // 前  0
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // 前  1
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  // 前  2
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  // 前  3

     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // 后  4
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // 后  5
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // 后  6
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  // 后  7

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  // 左  8
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  // 左  9
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  // 左  10
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  // 左  11

     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // 右  12
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  // 右  13
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // 右  14
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  // 右  15

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  // 上  16
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f,  // 上  17
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f,  // 上  18
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  // 上  19

     0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  // 下  20
    -0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  // 下  21
    -0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  // 下  22
     0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  // 下  23
};

static GLuint cubeIndices[] = {
     0,  1,  2,
     2,  3,  0,

     4,  5,  6,
     6,  7,  4,
    
     8,  9, 10,
    10, 11,  8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20,
};

// cube 负责生成一个立方体对象
// 主要的作用就是提供数据
class Cube {
public:
    Cube() : _VAO(0) {}
    ~Cube() {}
    bool init();
    GLuint getVAO() const { return _VAO; }
    
private:
    GLuint _VAO;           // VAO 对象
    GLuint _VBO;           // VBO 对象
    GLuint _EBO;           // EBO 对象
};

bool Cube::init() {
    // create VAO
    OpenGLInstance->glGenVertexArrays(1, &_VAO);

    // create VBO
    OpenGLInstance->glGenBuffers(1, &_VBO);

    // create EBO
    OpenGLInstance->glGenBuffers(1, &_EBO);

    // bind VAO
    OpenGLInstance->glBindVertexArray(_VAO);

    // bind VBO
    OpenGLInstance->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    // VAO buffer data
    OpenGLInstance->glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerices), cubeVerices, GL_STATIC_DRAW);
    // position attribute
    OpenGLInstance->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    OpenGLInstance->glEnableVertexAttribArray(0);
    // color attribute
    OpenGLInstance->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    OpenGLInstance->glEnableVertexAttribArray(1);


    // bind EBO
    OpenGLInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    // EBO buffer data
    OpenGLInstance->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);



    return true;
}

//static float triangleVertices[] = {
//     0.0f,  0.5f, 0.0f,  // 顶点
//    -0.5f, -0.5f, 0.0f,  // 左下角
//     0.5f, -0.5f, 0.0f,  // 右下角
//};
//
//static GLuint triangleIndices[] = {
//    0, 1, 2,  // 第一个三角形
//};


TWindow::TWindow()
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)),
    deltaTime(0.1f)
{
    setCursor(QCursor(Qt::BlankCursor));
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

    // create VBO
    OpenGLInstance->glGenBuffers(1, &VBO2);

    // create EBO
    OpenGLInstance->glGenBuffers(1, &EBO2);

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
    // position attribute
    OpenGLInstance->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    OpenGLInstance->glEnableVertexAttribArray(0);
    // color attribute
    OpenGLInstance->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    OpenGLInstance->glEnableVertexAttribArray(1);

    //OpenGLInstance->glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //OpenGLInstance->glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColor), cubeColor, GL_STATIC_DRAW);
    //OpenGLInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    //OpenGLInstance->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeColorIndices), cubeColorIndices, GL_STATIC_DRAW);
    //OpenGLInstance->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //OpenGLInstance->glEnableVertexAttribArray(1);



    // configure global opengl state
    // -----------------------------
    OpenGLInstance->glEnable(GL_DEPTH_TEST);
    OpenGLInstance->glEnable(GL_CULL_FACE);
    // 线框模式
    // OpenGLInstance->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 3. init window size
    resize(QSize(800, 800));
}

void TWindow::resizeGL(int w, int h) {
    OpenGLInstance->glViewport(0, 0, w, h);

    center.setX(w/2);
    center.setY(h/2);

    QCursor::setPos(mapToGlobal(center));

    update();
}

void TWindow::paintGL() {
    // clear background
    OpenGLInstance->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    OpenGLInstance->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 1.0f, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    // use shader program
    shader.use();
    shader.setMat4("projection", projection); 
    shader.setMat4("view", view);
    shader.setMat4("model", model);
    // bind vertex array object
    OpenGLInstance->glBindVertexArray(VAO);
    OpenGLInstance->glDrawElements(GL_TRIANGLES, sizeof(cubeIndices)/sizeof(GLuint), GL_UNSIGNED_INT, (GLvoid*)0);

    // release VAO
    OpenGLInstance->glBindVertexArray(0);
}

void TWindow::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        close();
        return;
    }

    // 前后移https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/1.colors/colors.cpp动
    if (e->key() == Qt::Key_W) {
        camera.processKeyboard(FORWARD, deltaTime);
    } else if (e->key() == Qt::Key_S) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }

    // 左右移动
    if (e->key() == Qt::Key_A) {
        camera.processKeyboard(LEFT, deltaTime);
    } else if (e->key() == Qt::Key_D) {
        camera.processKeyboard(RIGHT, deltaTime); 
    }

    update();
}

void TWindow::mouseMoveEvent(QMouseEvent *e) {
    GLint xoffset = e->x() - center.x();
    GLint yoffset = e->y() - center.y();

    QCursor::setPos(mapToGlobal(center));

    if (xoffset == 0 && yoffset == 0) { return; }
    if (xoffset > 100 || yoffset > 100) { return; }

    camera.processMouseMovement(xoffset, yoffset);

    update();
}

void TWindow::wheelEvent(QWheelEvent* e) {
    camera.processMouseScroll(e->delta() / 100.0f);
    update();
}
