#include "twindow.h"

#include <iostream>
#include <vector>

#include "utils/Camera.h"
#include "utils/OpenGLSingleton.h"
#include "utils/Shader.h"
#include "utils/utDefer.h"


static std::vector<GLfloat> cubeVertices = {
     // position         // color            // normal            // texture coords
     0.5f,  0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,        // 前  0
    -0.5f,  0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,        // 前  1
    -0.5f, -0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,        // 前  2
     0.5f, -0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,        // 前  3

    -0.5f,  0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,        // 后  4
     0.5f,  0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,        // 后  5
     0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,        // 后  6
    -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,        // 后  7

    -0.5f,  0.5f,  0.5f, 1.0f, 0.5f, 0.31f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,        // 左  8
    -0.5f,  0.5f, -0.5f, 1.0f, 0.5f, 0.31f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,        // 左  9
    -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.31f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,        // 左  10
    -0.5f, -0.5f,  0.5f, 1.0f, 0.5f, 0.31f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,        // 左  11

     0.5f,  0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,        // 右  12
     0.5f,  0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,        // 右  13
     0.5f, -0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,        // 右  14
     0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,        // 右  15

     0.5f,  0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,        // 上  16
    -0.5f,  0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,        // 上  17
    -0.5f,  0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,        // 上  18
     0.5f,  0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,        // 上  19

    -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,        // 下  20
     0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.31f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,        // 下  21
     0.5f, -0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,        // 下  22
    -0.5f, -0.5f,  0.5f, 1.0f, 0.5f, 0.31f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,        // 下  23
};

static std::vector<GLfloat> lightCubeVertices = {
     // position         
     0.5f,  0.5f,  0.5f,  // 前  0
    -0.5f,  0.5f,  0.5f,  // 前  1
    -0.5f, -0.5f,  0.5f,  // 前  2
     0.5f, -0.5f,  0.5f,  // 前  3

    -0.5f,  0.5f, -0.5f,  // 后  4
     0.5f,  0.5f, -0.5f,  // 后  5
     0.5f, -0.5f, -0.5f,  // 后  6
    -0.5f, -0.5f, -0.5f,  // 后  7

    -0.5f,  0.5f,  0.5f,  // 左  8
    -0.5f,  0.5f, -0.5f,  // 左  9
    -0.5f, -0.5f, -0.5f,  // 左  10
    -0.5f, -0.5f,  0.5f,  // 左  11

     0.5f,  0.5f, -0.5f,  // 右  12
     0.5f,  0.5f,  0.5f,  // 右  13
     0.5f, -0.5f,  0.5f,  // 右  14
     0.5f, -0.5f, -0.5f,  // 右  15

     0.5f,  0.5f, -0.5f,  // 上  16
    -0.5f,  0.5f, -0.5f,  // 上  17
    -0.5f,  0.5f,  0.5f,  // 上  18
     0.5f,  0.5f,  0.5f,  // 上  19

    -0.5f, -0.5f, -0.5f,  // 下  20
     0.5f, -0.5f, -0.5f,  // 下  21
     0.5f, -0.5f,  0.5f,  // 下  22
    -0.5f, -0.5f,  0.5f,  // 下  23
};

static std::vector<GLuint> cubeIndices = {
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
// layout = 0 : position
class Cube {
public:
    Cube(const std::vector<GLfloat>& cubeVertices, const std::vector<GLuint>& cubeIndices)
        : _VAO(0), _cubeVertices(cubeVertices), _cubeIndices(cubeIndices) {}
    virtual ~Cube() {}
    virtual bool init();
    bool draw();
    GLuint getVAO() const { return _VAO; }
    
protected:
    GLuint _VAO;             // VAO 对象
    GLuint _VBO;             // VBO 对象
    GLuint _EBO;             // EBO 对象

    const std::vector<GLfloat> _cubeVertices;
    const std::vector<GLuint>  _cubeIndices;
};

class LightCube : public Cube {
public:
    LightCube(const std::vector<GLfloat>& cubeVertices, const std::vector<GLuint>& cubeIndices)
        : Cube(cubeVertices, cubeIndices) {}

    ~LightCube() override {}

    bool init () override;

    void setPosition(const glm::vec3& v) { _position = v; }
    void setAmbient(const glm::vec3& v) { _ambient = v; }
    void setDiffuse(const glm::vec3& v) { _diffuse = v; }
    void setSpecular(const glm::vec3& v) { _specular = v; }

    const glm::vec3& getPosition() const { return _position; }
    const glm::vec3& getAmbient() const { return _ambient; }
    const glm::vec3& getDiffuse() const { return _diffuse; }
    const glm::vec3& getSpecular() const { return _specular; }


private:
    glm::vec3 _position;
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
};

static Shader cubeShader;        // 着色器
static Cube   cube(cubeVertices, cubeIndices);

static Shader    lightCubeShader;
static LightCube lightCube(lightCubeVertices, cubeIndices);

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
    OpenGLInstance->glBufferData(GL_ARRAY_BUFFER, _cubeVertices.size() * sizeof(GLfloat), &_cubeVertices[0], GL_STATIC_DRAW);
    // position attribute
    OpenGLInstance->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    OpenGLInstance->glEnableVertexAttribArray(0);
    // color attribute
    OpenGLInstance->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    OpenGLInstance->glEnableVertexAttribArray(1);
    // normal attribute
    OpenGLInstance->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    OpenGLInstance->glEnableVertexAttribArray(2);

    // bind EBO
    OpenGLInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    // EBO buffer data
    OpenGLInstance->glBufferData(GL_ELEMENT_ARRAY_BUFFER, _cubeIndices.size() * sizeof(GLuint), &_cubeIndices[0], GL_STATIC_DRAW);

    return true;
}

bool Cube::draw() {
    // bind vertex array object
    OpenGLInstance->glBindVertexArray(_VAO);
    OpenGLInstance->glDrawElements(GL_TRIANGLES, _cubeIndices.size(), GL_UNSIGNED_INT, (GLvoid*)0);

    // release VAO
    OpenGLInstance->glBindVertexArray(0);

    return true;
}

bool LightCube::init() {
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
    OpenGLInstance->glBufferData(GL_ARRAY_BUFFER, _cubeVertices.size() * sizeof(GLfloat), &_cubeVertices[0], GL_STATIC_DRAW);
    // position attribute
    OpenGLInstance->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    OpenGLInstance->glEnableVertexAttribArray(0);

    // bind EBO
    OpenGLInstance->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    // EBO buffer data
    OpenGLInstance->glBufferData(GL_ELEMENT_ARRAY_BUFFER, _cubeIndices.size() * sizeof(GLuint), &_cubeIndices[0], GL_STATIC_DRAW);

    // 设置点光源的位置
    setPosition(glm::vec3(1.2f, 1.0f, 2.0f));

    // 设置点光源的ambient
    setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));

    // 设置点光源的diffuse
    setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));

    // 设置点光源的specular
    setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

    return true;
}

TWindow::TWindow()
    : camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90),
    deltaTime(0.1f), initCursor(false)
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

    // 1. init window size
    resize(QSize(800, 800));
    
    // 2. configure global opengl state
    // -----------------------------
    OpenGLInstance->glEnable(GL_DEPTH_TEST);
    OpenGLInstance->glEnable(GL_CULL_FACE);
    // 线框模式
    // OpenGLInstance->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 3. init cubeShader
    if (!cubeShader.init("simpleVertexShader.glsl", "simpleFragmentShader.glsl")) {
        close();
        return;
    }

    // 4. cube init
    cube.init();

    // 5. init light cube shader
    if (!lightCubeShader.init("light_cube_vs.glsl", "light_cube_fs.glsl")) {
        close();
        return;
    }

    // 6. light cube init
    lightCube.init();
}

void TWindow::resizeGL(int w, int h) {
    OpenGLInstance->glViewport(0, 0, w, h);

    center.setX(w/2);
    center.setY(h/2);

    QCursor::setPos(mapToGlobal(center));
    initCursor = true;

    update();
}

void TWindow::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Escape) {
        close();
        return;
    }

    // 前后移动
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
    if (!initCursor) return;

    GLint xoffset = e->x() - center.x();
    GLint yoffset = center.y() - e->y();

    QCursor::setPos(mapToGlobal(center));

    if (xoffset == 0 && yoffset == 0) { return; }
    if (xoffset > 100 || yoffset > 100) { 
        update(); return; 
    }

    camera.processMouseMovement(xoffset, yoffset);

    update();
}

void TWindow::wheelEvent(QWheelEvent* e) {
    camera.processMouseScroll(e->delta() / 100.0f);
    update();
}

void TWindow::paintGL() {
    // clear background
    OpenGLInstance->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    OpenGLInstance->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw light
    {
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 1.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightCube.getPosition());
        model = glm::scale(model,glm::vec3(0.3f, 0.3f, 0.3f));

        lightCubeShader.use(); 
        lightCubeShader.setMat4("projection", projection); 
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setMat4("model", model);
        lightCubeShader.setVec3("lightColor", lightCube.getSpecular());

        lightCube.draw();
    }

    // draw cube
    {
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), 1.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        cubeShader.use();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("model", model);
        cubeShader.setVec3("viewPos", camera.getPosition());
        cubeShader.setVec3("light.position", lightCube.getPosition());
        cubeShader.setVec3("light.ambient", lightCube.getAmbient());
        cubeShader.setVec3("light.diffuse", lightCube.getDiffuse());
        cubeShader.setVec3("light.specular", lightCube.getSpecular());
        cubeShader.setVec3("material.ambient",  1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        cubeShader.setFloat("material.shininess", 32.0f);

        cube.draw();
    }

    // release VAO
    OpenGLInstance->glBindVertexArray(0);
}

