#include "mainwindow.h"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

#include <QDateTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>

static std::vector<GLfloat> planeVertices =
{
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

static std::vector<GLfloat> cubeVertices =
{
    // positions          // texture Coords
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

static std::vector<GLfloat> grassVertices =
{
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

static std::vector<GLfloat> windowVertices = grassVertices;

static std::vector<GLfloat> quadVertices =
{
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};


VertexDataBase:: VertexDataBase(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas)
    : _glCore(glCore), _datas(datas), _VAO(0), _VBO(0)
{
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

VertexDataBase::~VertexDataBase()
{
    _glCore->glDeleteBuffers(1, &_VBO);
    _glCore->glDeleteVertexArrays(1, &_VAO);
}


QuadVertexData::QuadVertexData(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas)
    : VertexDataBase(glCore, datas)
{
    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _datas.size(),
                          &(_datas[0]), GL_STATIC_DRAW);

    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

QuadVertexData::~QuadVertexData()
{

}

PlaneVertexData::PlaneVertexData(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas)
    : VertexDataBase(glCore, datas)
{
    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _datas.size(), &(_datas[0]), GL_STATIC_DRAW);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    _glCore->glBindVertexArray(0);
}

PlaneVertexData::~PlaneVertexData()
{

}

CubeVertexData::CubeVertexData(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas)
    : VertexDataBase(glCore, datas)
{
    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _datas.size(), &(_datas[0]), GL_STATIC_DRAW);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    _glCore->glBindVertexArray(0);
}

CubeVertexData::~CubeVertexData()
{
}

DepthScene::DepthScene(QOpenGLFunctions_3_3_Core* glCore) : _glCore(glCore)
{
    // marble.jpg
    _planeTexture = TextureFromFile(_glCore, "marble.jpg");
    _cubeTexture  = TextureFromFile(_glCore, "metal.png");
    _depthShader = new Shader(glCore);
    _depthShader->init("depth.vs", "depth.fs");
    _planeVertexData = new PlaneVertexData(_glCore, planeVertices);
    _cubeVertexData = new CubeVertexData(_glCore, cubeVertices);
}

DepthScene::~DepthScene()
{
    _glCore->glDeleteTextures(1, &_planeTexture);
    _glCore->glDeleteTextures(1, &_cubeTexture);
    _SAFE_DEL(_planeVertexData);
    _SAFE_DEL(_cubeVertexData);
    _SAFE_DEL(_depthShader);
}

void DepthScene::paint(Camera* camera, const glm::mat4& projection)
{
    _glCore->glEnable(GL_DEPTH_TEST);

    _glCore->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    _glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _depthShader->use();

    _depthShader->setMat4("view", 1, GL_FALSE, camera->getViewMatrix());
    _depthShader->setMat4("projection", 1, GL_FALSE, projection);

    auto model = glm::mat4(1.0f);

    // draw cubes
    _glCore->glBindVertexArray(_cubeVertexData->getVAO());
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _cubeTexture);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.001f, -1.0f));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.001f, 0.0f));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw plane
    _glCore->glBindVertexArray(_planeVertexData->getVAO());
    _glCore->glBindTexture(GL_TEXTURE_2D, _planeTexture);
    _depthShader->setMat4("model", 1, GL_FALSE, glm::mat4(1.0f));
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);

}

DepthScene2::DepthScene2(QOpenGLFunctions_3_3_Core* glCore)
    : DepthScene(glCore)
{
    _contianerTexture = TextureFromFile(_glCore, "container.jpg");
}

DepthScene2::~DepthScene2()
{
    _glCore->glDeleteTextures(1, &_contianerTexture);

}

void DepthScene2::paint(Camera* camera, const glm::mat4& projection)
{
    _glCore->glEnable(GL_DEPTH_TEST);

    _glCore->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    _glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _depthShader->use();

    _depthShader->setMat4("view", 1, GL_FALSE, camera->getViewMatrix());
    _depthShader->setMat4("projection", 1, GL_FALSE, projection);

    auto model = glm::mat4(1.0f);

    // draw cubes
    _glCore->glBindVertexArray(_cubeVertexData->getVAO());
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _contianerTexture);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.001f, -1.0f));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.001f, 0.0f));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw plane
    _glCore->glBindVertexArray(_planeVertexData->getVAO());
    _glCore->glBindTexture(GL_TEXTURE_2D, _planeTexture);
    _depthShader->setMat4("model", 1, GL_FALSE, glm::mat4(1.0f));
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);

}

ObjectOutlingScene::ObjectOutlingScene(QOpenGLFunctions_3_3_Core* glCore)
    : DepthScene(glCore)
{
    _normalShader = new Shader(_glCore);
    _normalShader->init("stencil_testing.vs", "stencil_testing.fs");
    _singleColorShader = new Shader(_glCore);
    _singleColorShader->init("stencil_testing.vs", "stencil_single_color.fs");
}

ObjectOutlingScene::~ObjectOutlingScene()
{
    _SAFE_DEL(_normalShader);
    _SAFE_DEL(_singleColorShader);
}

void ObjectOutlingScene::paint(Camera* camera, const glm::mat4& projection)
{
    _glCore->glEnable(GL_DEPTH_TEST);
    _glCore->glEnable(GL_STENCIL_TEST);
    _glCore->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    _glCore->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    _glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    auto model = glm::mat4(1.0f);

    _normalShader->use();
    _normalShader->setMat4("view", 1, GL_FALSE, camera->getViewMatrix());
    _normalShader->setMat4("projection", 1, GL_FALSE, projection);

    _glCore->glActiveTexture(GL_TEXTURE0);

    // draw floor
    _glCore->glStencilMask(0x00);
    _glCore->glBindTexture(GL_TEXTURE0, _planeTexture);
    _normalShader->setInt("texture0", 0);
    _normalShader->setMat4("model", 1, GL_FALSE, glm::mat4(1));
    _glCore->glBindVertexArray(_planeVertexData->getVAO());
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);

    // normal draw tow containers
    _glCore->glStencilMask(0xFF);
    _glCore->glStencilFunc(GL_ALWAYS, 1, 0xFF);
    _glCore->glBindVertexArray(_cubeVertexData->getVAO());
    _glCore->glBindTexture(GL_TEXTURE_2D, _cubeTexture);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.001f, -1.0f));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.001f, 0.0f));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);

    _glCore->glDisable(GL_DEPTH_TEST);
    _glCore->glStencilMask(0x00);
    _glCore->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    float scale = 1.1f;
    _singleColorShader->use();
    _singleColorShader->setMat4("view", 1, GL_FALSE, camera->getViewMatrix());
    _singleColorShader->setMat4("projection", 1, GL_FALSE, projection);
    _glCore->glBindVertexArray(_cubeVertexData->getVAO());
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.001f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.001f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    _depthShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);

    _glCore->glEnable(GL_DEPTH_TEST);
    _glCore->glStencilMask(0xFF);
    _glCore->glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

Blending1Scene::Blending1Scene(QOpenGLFunctions_3_3_Core* glCore)
    : DepthScene(glCore),
      _vegetation({
                  glm::vec3(-1.5f, 0.0f, -0.48f),
                  glm::vec3( 1.5f, 0.0f, 0.51f),
                  glm::vec3( 0.0f, 0.0f, 0.7f),
                  glm::vec3(-0.3f, 0.0f, -2.3f),
                  glm::vec3 (0.5f, 0.0f, -0.6f)
              })
{
    _grassShader = new Shader(_glCore);
    _grassShader->init("blending1.vs", "blending1.fs");
    _grassTexture = TextureFromFile(_glCore, "grass.png");
    _glCore->glBindTexture(GL_TEXTURE_2D, _grassTexture);
    _glCore->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _glCore->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    _glCore->glBindTexture(GL_TEXTURE_2D, 0);
    _grassVertexData = new GrassVertexData(_glCore, grassVertices);
}

Blending1Scene::~Blending1Scene()
{
    _glCore->glDeleteTextures(1, &_grassTexture);
    _SAFE_DEL(_grassVertexData);
    _SAFE_DEL(_grassShader);
}

void Blending1Scene::paint(Camera* camera, const glm::mat4& projection)
{
    _glCore->glEnable(GL_DEPTH_TEST);

    _glCore->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    _glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _grassShader->use();

    _grassShader->setMat4("view", 1, GL_FALSE, camera->getViewMatrix());
    _grassShader->setMat4("projection", 1, GL_FALSE, projection);

    auto model = glm::mat4(1.0f);

    // draw cubes
    _glCore->glBindVertexArray(_cubeVertexData->getVAO());
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _cubeTexture);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.001f, -1.0f));
    _grassShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.001f, 0.0f));
    _grassShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw plane
    _glCore->glBindVertexArray(_planeVertexData->getVAO());
    _glCore->glBindTexture(GL_TEXTURE_2D, _planeTexture);
    _grassShader->setMat4("model", 1, GL_FALSE, glm::mat4(1.0f));
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw grass
    _glCore->glBindVertexArray(_grassVertexData->getVAO());
    _glCore->glBindTexture(GL_TEXTURE_2D, _grassTexture);
    for (size_t i=0; i<_vegetation.size(); ++i)
    {
        model = glm::translate(glm::mat4(1.0f), _vegetation[i]);
        _grassShader->setMat4("model", 1, GL_FALSE, model);
        _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

Blending2Scene::Blending2Scene(QOpenGLFunctions_3_3_Core* glCore)
    : Blending1Scene(glCore)
{
    _windowTexture = TextureFromFile(_glCore, "window.png");
    _windowShader = new Shader(_glCore);
    _windowShader->init("blending2.vs", "blending2.fs");
    _windowVertexData = new WindowVertexData(_glCore, windowVertices);
}

Blending2Scene::~Blending2Scene()
{
    _glCore->glDeleteTextures(1, &_windowTexture);
    _SAFE_DEL(_windowShader);
    _SAFE_DEL(_windowVertexData);
}

void Blending2Scene::paint(Camera* camera, const glm::mat4& projection)
{
    defer(_glCore->glBindVertexArray(0));

    _glCore->glEnable(GL_DEPTH_TEST);
    _glCore->glEnable(GL_BLEND);
    _glCore->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::sort(_vegetation.begin(), _vegetation.end(),
              [&](const glm::vec3& first, const glm::vec3& second) {
        return glm::length(camera->getPosition() - first) > glm::length(camera->getPosition() - second);
    });

    _glCore->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    _glCore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _glCore->glActiveTexture(GL_TEXTURE0);

    // draw flow
    _windowShader->use();
    _glCore->glBindTexture(GL_TEXTURE_2D, _planeTexture);
    _windowShader->setInt("texture0", 0);
    _windowShader->setMat4("view", 1, GL_FALSE, camera->getViewMatrix());
    _windowShader->setMat4("projection", 1, GL_FALSE, projection);
    _windowShader->setMat4("model", 1, GL_FALSE, glm::mat4(1.0));
    _glCore->glBindVertexArray(_planeVertexData->getVAO());
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);

    auto model = glm::mat4(1.0f);
    // draw cubes
    _glCore->glBindTexture(GL_TEXTURE_2D, _cubeTexture);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.001f, -1.0f));
    _windowShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glBindVertexArray(_cubeVertexData->getVAO());
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.001f, 0.0f));
    _windowShader->setMat4("model", 1, GL_FALSE, model);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw windows
    _glCore->glBindTexture(GL_TEXTURE_2D, _windowTexture);
    _glCore->glBindVertexArray(_windowVertexData->getVAO());
    for (size_t i =0; i<_vegetation.size(); ++i)
    {
        model = glm::translate(glm::mat4(1.0f), _vegetation[i]);
        _windowShader->setMat4("model", 1, GL_FALSE, model);
        _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);
    }

}


FrameBufferScene::FrameBufferScene(QOpenGLFunctions_3_3_Core* glCore)
    : DepthScene2(glCore)
{
    _glCore->glGenFramebuffers(1, &_FBO);

    _glCore->glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    defer(_glCore->glBindFramebuffer(GL_FRAMEBUFFER, 0));

    _glCore->glGenTextures(1, &_colorBuf);
    _glCore->glBindTexture(GL_TEXTURE_2D, _colorBuf);
    defer(_glCore->glBindTexture(GL_TEXTURE_2D, 0));
    _glCore->glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    _glCore->glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuf, 0);


    _glCore->glGenRenderbuffers(1, &_RBO);
    _glCore->glBindRenderbuffer(GL_RENDERBUFFER, _RBO);
    defer(_glCore->glBindRenderbuffer(GL_RENDERBUFFER, 0));
    _glCore->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
    _glCore->glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _RBO);

    if (_glCore->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    _quadShader = new Shader(_glCore);
    _quadShader->init("framebuffers_screen.vs", "framebuffers_screen.fs");
    _quadVertexData = new QuadVertexData(_glCore, quadVertices);
}

FrameBufferScene::~FrameBufferScene()
{
    _glCore->glDeleteFramebuffers(1, &_FBO);
    _glCore->glDeleteTextures(1, &_colorBuf);
    _glCore->glDeleteRenderbuffers(1, &_RBO);
    _SAFE_DEL(_quadShader);
    _SAFE_DEL(_quadVertexData);
}

void FrameBufferScene::paint(Camera* camera, const glm::mat4& projection)
{
    _glCore->glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    defer(_glCore->glBindFramebuffer(GL_FRAMEBUFFER, 0));

    DepthScene2::paint(camera, projection);

    _glCore->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _glCore->glDisable(GL_DEPTH_TEST);
    defer(_glCore->glEnable(GL_DEPTH_TEST));
    _glCore->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    _glCore->glClear(GL_COLOR_BUFFER_BIT);
    _quadShader->use();
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _colorBuf);
    _glCore->glBindVertexArray(_quadVertexData->getVAO());
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 6);
    //_glCore->glDrawArrays(GL_LINES, 0, 6);
}

MainWindow::MainWindow() :
    _projection(1.0f)
{
    resize(800, 800);

    _camera = new Camera(glm::vec3(0.0f, 0.0f, 8.0f));
}

MainWindow::~MainWindow()
{
    _SAFE_DEL(_camera);
    _SAFE_DEL(_modelShader);
    _SAFE_DEL(_model);

    _SAFE_DEL(_depthScene);

    _SAFE_DEL(_objectOutlingScene);

    _SAFE_DEL(_blending1Scene);

    _SAFE_DEL(_blending2Scene);

    _SAFE_DEL(_frameBufferScene);
}

void MainWindow::initializeGL()
{
    if (!initializeOpenGLFunctions())
    {
        std::cerr << "initializeOpenGLFunctions failed!" << std::endl;
    }

    _modelShader = new Shader(this);
    _modelShader->init(_modelVsFile, _modelFsFile);

    _model = new Model(this, "backpack/backpack.obj");

    _depthScene = new DepthScene(this);

    _objectOutlingScene = new ObjectOutlingScene(this);

    _blending1Scene = new Blending1Scene(this);

    _blending2Scene = new Blending2Scene(this);

    _frameBufferScene = new FrameBufferScene(this);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&](){
        update();
    });
    timer->start(20);
}

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    _projection = glm::perspective(glm::radians(45.0f), (float)(h/h), 0.1f, 100.0f);

    resetCursor();

    update();
}

void MainWindow::paintGL()
{
    //_depthScene->paint(_camera, _projection);
    //_objectOutlingScene->paint(_camera, _projection);
    //_blending1Scene->paint(_camera, _projection);
    //_blending2Scene->paint(_camera, _projection);
    _frameBufferScene->paint(_camera, _projection);
}

void MainWindow::drawMoel()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _modelShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
    _modelShader->setMat4("projection", 1, GL_FALSE, _projection);

    _model->Draw(*_modelShader);

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    auto deltaTime = [&]() -> GLfloat {
        QDateTime currentTime = QDateTime::currentDateTime();
        static QDateTime lastTime = currentTime.addMSecs(-30);
        defer(lastTime = currentTime);
        if (lastTime.msecsTo(currentTime) > 100) return 30 / 1000.0f;
        return lastTime.msecsTo(currentTime) / 1000.0f;
    };

    switch (e->key())
    {
    case Qt::Key_W:
        _camera->processKeyboard(Camera::Movement::FORWARD, deltaTime());
        break;
    case Qt::Key_S:
        _camera->processKeyboard(Camera::Movement::BACKWARD, deltaTime());
        break;
    case Qt::Key_A:
        _camera->processKeyboard(Camera::Movement::LEFT, deltaTime());
        break;
    case Qt::Key_D:
        _camera->processKeyboard(Camera::Movement::RIGHT, deltaTime());
        break;
    case Qt::Key_Escape:
        close();
        break;
    default:
        break;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (!isActive()) return;
    if (e->x() == width()/2 && e->y() == height()/2) return;
    GLint xoffset = width()/2 - e->x();
    GLint yoffset = height()/2 - e->y();
    resetCursor();
    _camera->processMouseMovement(xoffset, yoffset);
}

void MainWindow::focusInEvent(QFocusEvent *)
{
    resetCursor();
}


void MainWindow::resetCursor()
{
    QCursor cursor = this->cursor();
    cursor.setPos(mapToGlobal(QPoint(width()/2, height()/2)));
    cursor.setShape(Qt::CursorShape::BlankCursor);
    this->setCursor(cursor);
}

