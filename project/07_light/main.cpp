#include <iostream>
#include <functional>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/stb_image.h"

#include "utils/utDefer.h"
#include "utils/Shader.h"
#include "utils/Camera.h"
#include "utils/Singleton.h"

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

class Cube {
public:
    Cube();
    ~Cube();

    bool init();
    bool destory();
    bool draw();

private:
    std::vector<float>* _vertices;

    unsigned int VAO;
    unsigned int VBO;
};

Cube::Cube() : _vertices(NULL) {}

Cube::~Cube() {
    destory();
}

bool Cube::init() {
    _vertices = new std::vector<float> {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return true;
}

bool Cube::destory() {
    if (_vertices) delete _vertices;

    glDeleteBuffers(1, &VAO);
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
    void setWidth(int width) { _winWidth = width; }
    void setHeight(int height) { _winHeight = height; }

private:
    void processInput(GLFWwindow* window);

private:
    bool initGLFW();
    bool initOpenGLVersion();
    bool createWindow();
    bool initGLEW();

private:
    bool _isInit = false;

    int  _winHeight = 600;
    int  _winWidth = 800;
    const char* _winName = "LearnOpenGL";
    
    GLFWwindow* _window = NULL;

    Camera _camera;

    float _deltaTime = 0.0f;
    float _lastFrame = 0.0f;
    Shader _ourShader;

    Cube _cube;
    ContainerTexture _containerTex;
    SmileTexture _smileTex;
};

#define OpenGLWindowInstance (*(OpenGLWindow::getInstancePtr()))

OpenGLWindow::OpenGLWindow()
    : _camera(glm::vec3(0.0f, 0.0f, 3.0f)) {
}

OpenGLWindow::~OpenGLWindow() {
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

    _isInit = true;
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

bool OpenGLWindow::init() {
    if (!initGLFW()) return false;

    if (!createWindow()) return false;

    initOpenGLVersion();

    if (!initGLEW()) return false;

    // init graphic
    if (!_containerTex.load("pic/container.jpg")) return false;
    if (!_smileTex.load("pic/awesomeface.png")) return false;

    _ourShader.init("simpleVertex.glsl", "simpleFragment.glsl");

    _cube.init();

    return true;
}

bool OpenGLWindow::run() {
	static glm::vec3 cubePositions[] = {
	  glm::vec3( 0.0f,  0.0f,  0.0f), 
	  glm::vec3( 2.0f,  5.0f, -15.0f), 
	  glm::vec3(-1.5f, -2.2f, -2.5f),  
	  glm::vec3(-3.8f, -2.0f, -12.3f),  
	  glm::vec3( 2.4f, -0.4f, -3.5f),  
	  glm::vec3(-1.7f,  3.0f, -7.5f),  
	  glm::vec3( 1.3f, -2.0f, -2.5f),  
	  glm::vec3( 1.5f,  2.0f, -2.5f), 
	  glm::vec3( 1.5f,  0.2f, -1.5f), 
	  glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

    glEnable(GL_DEPTH_TEST);
    
    while (!glfwWindowShouldClose(_window)) {
        float currentFrame = glfwGetTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;
        processInput(_window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // 4. 绘制物体
        _ourShader.use();
        _ourShader.setInt("texture1", 0);
        _ourShader.setInt("texture2", 1);

        glm::mat4 view = glm::mat4(1.0f);
        view = _camera.getViewMatrix();
        _ourShader.setMat4("view", view);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(_camera.getZoom()), 1.0f * _winWidth /
                _winHeight, 0.1f, 100.0f);
        _ourShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _containerTex.getId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _smileTex.getId());

        for (int i=0; i<10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            _ourShader.setMat4("model", model);

            _cube.draw();
        }

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    return true;
}

void OpenGLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    OpenGLWindowInstance.setWidth(width);
    OpenGLWindowInstance.setHeight(height);
}

void OpenGLWindow::processInput(GLFWwindow* window) {
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

