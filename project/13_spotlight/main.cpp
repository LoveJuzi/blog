#include <iostream>
#include <functional>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/Camera.h"
#include "utils/Shader.h"
#include "utils/Singleton.h"
#include "utils/stb_image.h"
#include "utils/utDefer.h"

class LoadImage {
public:
    LoadImage();
    ~LoadImage();
    bool load(const std::string& imgfile);
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    unsigned char* getData() const { return _data; }
private:
    unsigned char* _data = NULL;
    int _width = 0;
    int _height = 0;
};

LoadImage::LoadImage() {}

LoadImage::~LoadImage() {
    if (_data) stbi_image_free(_data);
}

bool LoadImage::load(const std::string& imgfile) {
    stbi_set_flip_vertically_on_load(true);

    int nrChannels1;
    _data = stbi_load(imgfile.c_str(), &_width, &_height, &nrChannels1, 0);
    if (_data == NULL) {
        std::cerr << "Failed to laod texture" << std::endl;
        return false;
    }

    return true;
}

class ContainerTexture {
public:
    ContainerTexture() {}
    ~ContainerTexture();

    bool load(const std::string& picfile);
    bool destory();
    unsigned int getId() const { return _texid; }

private:
    unsigned int _texid;
};

ContainerTexture::~ContainerTexture() {
    destory();
}

bool ContainerTexture::load(const std::string& picfile) {
    LoadImage image;
    if (!image.load(picfile)) { return false; }

    glGenTextures(1, &_texid);

    glBindTexture(GL_TEXTURE_2D, _texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0,
            GL_RGB, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

bool ContainerTexture::destory() {
    glDeleteTextures(1, &_texid);
    return true;
}

class SmileTexture {
public:
    SmileTexture() {}
    ~SmileTexture();

    bool load(const std::string& picfile);
    bool destory();
    GLuint getId() const { return _texid; }

private:
    unsigned int _texid;
};

SmileTexture::~SmileTexture() {
    destory();
}

bool SmileTexture::load(const std::string& picfile) {
    LoadImage image;
    if (!image.load(picfile)) { return false; }

    glGenTextures(1, &_texid);

    glBindTexture(GL_TEXTURE_2D, _texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0,
            GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

bool SmileTexture::destory() {
    glDeleteTextures(1, &_texid);
    return true;
}

class PngTexture {
public:
    PngTexture() {}
    ~PngTexture();

    bool load(const std::string& picfile);
    bool destory();
    GLuint getId() const { return _texid; }

private:
    unsigned int _texid;
};

PngTexture::~PngTexture() {
    destory();
}

bool PngTexture::load(const std::string& picfile) {
    LoadImage image;
    if (!image.load(picfile)) { return false; }

    glGenTextures(1, &_texid);

    glBindTexture(GL_TEXTURE_2D, _texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0,
            GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

bool PngTexture::destory() {
    glDeleteTextures(1, &_texid);
    return true;
}

class Cube {
public:
    Cube();
    virtual ~Cube();

    bool init();
    bool destory();
    bool draw();

protected:
    std::vector<float>* _vertices;

    unsigned int VAO;
    unsigned int VBO;
};

class WoodenBox : public Cube {
public:
    WoodenBox();
    ~WoodenBox() override {}

    const glm::vec3& getAmbient() const { return _ambient; }
    const glm::vec3& getDiffuse() const { return _diffuse; }
    const glm::vec3& getSpecular() const { return _specular; }
    float getShininess() const { return _shininess; }

private:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    float     _shininess;
};

WoodenBox::WoodenBox()
    : _ambient(1.0f, 0.5f, 0.31f)
    , _diffuse(1.0f, 0.5f, 0.31f)
    , _specular(0.5f, 0.5f, 0.5f)
    , _shininess(32.0f)
{}

class LightCube : public Cube {
public:
    LightCube();
    ~LightCube() override {}

    const glm::vec3& getLightScale() const { return _lightScale; }
    const glm::vec3& getLightPos() const { return _lightPos; }
    const glm::vec3& getDircetion() const { return _direction; } 
    float getCutOff() const { return _cutOff; }
    float getOuterCutOff() const { return _outerCutOff; }
    const glm::vec3& getLightColor() const { return _lightColor; }

    const glm::vec3& getAmbient() const { return _ambient; }
    const glm::vec3& getDiffuse() const { return _diffuse; }
    const glm::vec3& getSpecular() const { return _specular; }

private:
    glm::vec3 _lightScale;
    glm::vec3 _lightPos;
    glm::vec3 _direction;
    float     _cutOff;
    float     _outerCutOff;

    glm::vec3 _lightColor;

    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
};

LightCube::LightCube()
    : _lightPos(1.2f, 1.0f, 2.0f)
    , _cutOff(glm::cos(glm::radians(12.5f)))
    , _outerCutOff(glm::cos(glm::radians(17.5f)))
    , _lightScale(0.2f)
    , _lightColor(1.0f, 1.0f, 1.0f)
    , _ambient(0.2f, 0.2f, 0.2f)
    , _diffuse(0.5f, 0.5, 0.5f)
    , _specular(1.0f, 1.0f, 1.0f)
{
}

Cube::Cube() : _vertices(NULL) {}

Cube::~Cube() {
    destory();
}

bool Cube::init() {
    _vertices = new std::vector<float> {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 复制顶点数组到缓冲中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(_vertices->size()), 
            &(*_vertices)[0], GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return true;
}

bool Cube::destory() {
    if (_vertices) delete _vertices;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return true;
}

bool Cube::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    return true;
}



// opengle singleton window
class OpenGLWindow : public Singleton<OpenGLWindow> {
private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

public:
    OpenGLWindow();
    ~OpenGLWindow();

    bool init();
    bool destory();
    bool run();

    Camera& camera() { return _camera; }

private:
    void processInput(GLFWwindow* window);

private:
    bool initGLFW();
    bool initOpenGLVersion();
    bool createWindow();
    bool initGLEW();

private:
    int  _winHeight = 800;
    int  _winWidth  = 800;
    const char* _winName = "LearnOpenGL";
    
    GLFWwindow* _window = NULL;

    Camera _camera;

    float _deltaTime = 0.0f;
    float _lastFrame = 0.0f;

    Shader     _lightingShader;
    WoodenBox  _woodenBox;
    PngTexture _container2Tex;
    PngTexture _containerSpecular2Tex;

    LightCube _lightCube;
    Shader    _lightCubeShader;
};

#define OpenGLWindowInstance (*(OpenGLWindow::getInstancePtr()))

OpenGLWindow::OpenGLWindow()
    : _camera(glm::vec3(0.0f, 0.0f, 5.0f)) {
}

OpenGLWindow::~OpenGLWindow() {
}

bool OpenGLWindow::init() {
    if (!initGLFW()) return false;

    if (!createWindow()) return false;

    initOpenGLVersion();

    if (!initGLEW()) return false;

    if (!_lightingShader.init("lightingVertex.glsl", "lightingFragment.glsl")) {
        return false;
    }
    if (!_woodenBox.init()) return false;
    if (!_container2Tex.load("pic/container2.png")) return false;
    if (!_containerSpecular2Tex.load("pic/container2_specular.png")) return false;

    if (!_lightCubeShader.init("lightCubeVertex.glsl", "lightCubeFragment.glsl")) {
        return false;
    }
    if (!_lightCube.init()) return false;

    return true;
}

bool OpenGLWindow::run() {
    // draw light
    auto drawLight = [&]() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, _lightCube.getLightPos());
        model = glm::scale(model, _lightCube.getLightScale());

        _lightCubeShader.use();
        _lightCubeShader.setMat4("projection", _camera.getProjection());
        _lightCubeShader.setMat4("view", _camera.getViewMatrix());
        _lightCubeShader.setMat4("model", model);
        _lightCubeShader.setVec3("lightColor", _lightCube.getLightColor());

        _lightCube.draw();
    };

    // draw wooden box
    auto drawWoodenBox = [&]() {
        glm::mat4 model = glm::mat4(1.0f);

        _lightingShader.use();
        _lightingShader.setMat4("projection", _camera.getProjection());
        _lightingShader.setMat4("view", _camera.getViewMatrix());
        _lightingShader.setMat4("model", model);
        _lightingShader.setVec3("viewPos", _camera.getPosition());
        _lightingShader.setInt("material.diffuse", 0);
        _lightingShader.setInt("material.specular", 1);
        _lightingShader.setFloat("material.shininess", _woodenBox.getShininess());
        _lightingShader.setVec3("light.ambient", _lightCube.getAmbient());
        _lightingShader.setVec3("light.diffuse", _lightCube.getDiffuse());
        _lightingShader.setVec3("light.specular", _lightCube.getSpecular());
        _lightingShader.setVec3("light.position", _camera.getPosition());
        _lightingShader.setVec3("light.direction", _camera.getFront());
        _lightingShader.setFloat("light.cutOff", _lightCube.getCutOff());
        _lightingShader.setFloat("light.outerCutOff", _lightCube.getOuterCutOff());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _container2Tex.getId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _containerSpecular2Tex.getId());

        _woodenBox.draw();

    };

    while (!glfwWindowShouldClose(_window)) {
        processInput(_window);

        // 启用深度测试
        glEnable(GL_DEPTH_TEST);
        // clear window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw
        // drawLight();
        drawWoodenBox();

        // swap buffer
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    return true;
}



bool OpenGLWindow::destory() {
    glfwTerminate();
    return true;
}

bool OpenGLWindow::initGLFW() {
    if (!glfwInit()) {
        std::cerr << "init glfw error!!!" << std::endl;
        return false;
    }

    return true;
}

bool OpenGLWindow::initOpenGLVersion() {
    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // use OpenGL core mode
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    return true;
}

bool OpenGLWindow::createWindow() {

    _window = glfwCreateWindow(_winWidth, _winHeight, _winName, NULL, NULL);
    if (_window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(_window);

    glfwSetFramebufferSizeCallback(_window, &OpenGLWindow::framebuffer_size_callback);

    glfwSetCursorPosCallback(_window, &OpenGLWindow::mouse_callback);

    glfwSetScrollCallback(_window, &OpenGLWindow::scroll_callback);

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

bool OpenGLWindow::initGLEW() {
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}

void OpenGLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    OpenGLWindowInstance.camera().setRatio(1.0 * width / height);
}

void OpenGLWindow::processInput(GLFWwindow* window) {
    _deltaTime = _lastFrame;
    _lastFrame = glfwGetTime();
    _deltaTime = _lastFrame - _deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        _camera.processKeyboard(CameraMovement::FORWARD, _deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        _camera.processKeyboard(CameraMovement::BACKWARD, _deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        _camera.processKeyboard(CameraMovement::LEFT, _deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        _camera.processKeyboard(CameraMovement::RIGHT, _deltaTime);
    }
}

void OpenGLWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = xpos;
    static float lastY = ypos;
    static float init = true;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    OpenGLWindowInstance.camera().processMouseMovement(xoffset, yoffset, init);
    init = false;
}

void OpenGLWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    OpenGLWindowInstance.camera().processMouseScroll(yoffset);
}

int main() {
    if (!OpenGLWindowInstance.init()) return -1;
    utDefer(OpenGLWindowInstance.destory());
    if (!OpenGLWindowInstance.run()) return -1;
    return 0;
}

