#include <iostream>
#include <functional>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/stb_image.h"

#include "utils/utDefer.h"
#include "utils/Shader.h"
#include "utils/Camera.h"

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

// deal opengl window
class OpenGLWindow {
private:
    static OpenGLWindow* windowInstance;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

public:
    OpenGLWindow();
    ~OpenGLWindow();

    bool init();
    bool run();

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

    unsigned int texture1;
    unsigned int texture2;
    unsigned int VBO;
	unsigned int EBO;
    unsigned int VAO;

    Camera camera;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    Shader ourShader;

    int screenWidth = 800;
    int screenHeight = 600;
};

OpenGLWindow* OpenGLWindow::windowInstance = NULL;

OpenGLWindow::OpenGLWindow()
    : camera(glm::vec3(0.0f, 0.0f, 3.0f)),
    cameraPos(glm::vec3(0.0f, 0.0f, 3.0f)), 
    cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
    cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)) {
}

OpenGLWindow::~OpenGLWindow() {
    if (_isInit) {
        glfwTerminate();
    }
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

    OpenGLWindow::windowInstance = this;

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
    LoadImage image1;
    if (!image1.load("pic/container.jpg")) return false;
    LoadImage image2;
    if (!image2.load("pic/awesomeface.png")) return false;

	static float vertices[] = {
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


	static unsigned int indices[] = {
        0, 1, 3,  // 第一个三角形
        1, 2, 3   // 第二个三角形
    };



    glEnable(GL_DEPTH_TEST);


    glGenBuffers(1, &VBO);
    // utDefer(glDeleteBuffers(1, &VBO));

    glGenBuffers(1, &EBO);
    // utDefer(glDeleteBuffers(1, &EBO));

    glGenVertexArrays(1, &VAO);
    // utDefer(glDeleteVertexArrays(1, &VAO));


    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image1.getWidth(), image1.getHeight(), 0,
            GL_RGB, GL_UNSIGNED_BYTE, image1.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2.getWidth(), image2.getHeight(), 0,
            GL_RGBA, GL_UNSIGNED_BYTE, image2.getData());
    glGenerateMipmap(GL_TEXTURE_2D);


    ourShader.init("simpleVertex.glsl", "simpleFragment.glsl");
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 复制顶点数组到缓冲中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
    
    while (!glfwWindowShouldClose(_window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(_window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // 4. 绘制物体
        ourShader.use();

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.getViewMatrix();
        ourShader.setMat4("view", view);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getZoom()), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        for (int i=0; i<10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    return true;
}

void OpenGLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void OpenGLWindow::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
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

    windowInstance->camera.processMouseMovement(xoffset, yoffset, init);
    init = false;
}

void OpenGLWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    windowInstance->camera.processMouseScroll(yoffset);
}

int main() {
    OpenGLWindow win;
    if (!win.init()) return -1;
    if (!win.run()) return -1;
    return 0;
}

