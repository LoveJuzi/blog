#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

#include "utils.h"

class VertexDataBase
{
public:
    VertexDataBase(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas);
    virtual ~VertexDataBase();

    GLuint getVAO() const { return _VAO; }

protected:
    QOpenGLFunctions_3_3_Core* _glCore;
    std::vector<GLfloat>       _datas;
    GLuint _VAO;
    GLuint _VBO;
};

class QuadVertexData : public VertexDataBase
{
public:
    QuadVertexData(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas);
    ~QuadVertexData() override;
};

class PlaneVertexData : public VertexDataBase
{
public:
    PlaneVertexData(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas);
    ~PlaneVertexData() override;
};
typedef PlaneVertexData GrassVertexData;
typedef PlaneVertexData WindowVertexData;

class CubeVertexData : public VertexDataBase
{
public:
    CubeVertexData(QOpenGLFunctions_3_3_Core* glCore, const std::vector<GLfloat>& datas);
    ~CubeVertexData() override;
};

class DepthScene
{
public:
    DepthScene(QOpenGLFunctions_3_3_Core* glCore);
    ~DepthScene();

    void paint(Camera* camera, const glm::mat4& projection);

protected:
    QOpenGLFunctions_3_3_Core* _glCore = NULL;

    Shader*          _depthShader = NULL;
    PlaneVertexData* _planeVertexData = NULL;
    CubeVertexData*  _cubeVertexData = NULL;

    GLuint _planeTexture = 0;
    GLuint _cubeTexture  = 0;
};

class DepthScene2 : public DepthScene
{
public:
    DepthScene2(QOpenGLFunctions_3_3_Core* glCore);
    ~DepthScene2();

    void paint(Camera* camera, const glm::mat4& projection);

protected:
    GLuint _contianerTexture = 0;
};

class ObjectOutlingScene : public DepthScene
{
public:
    ObjectOutlingScene(QOpenGLFunctions_3_3_Core* glCore);
    ~ObjectOutlingScene();

    void paint(Camera* camera, const glm::mat4& projection);

private:
    Shader* _normalShader = NULL;
    Shader* _singleColorShader = NULL;
};

class Blending1Scene : public DepthScene
{
public:
    Blending1Scene(QOpenGLFunctions_3_3_Core* glCore);
    ~Blending1Scene();

    void paint(Camera* camera, const glm::mat4& projection);

protected:
    GLuint _grassTexture = 0;

    Shader*                _grassShader = NULL;
    GrassVertexData*       _grassVertexData = NULL;
    std::vector<glm::vec3> _vegetation;
};

class Blending2Scene : public Blending1Scene
{
public:
    Blending2Scene(QOpenGLFunctions_3_3_Core* glCore);
    ~Blending2Scene();

    void paint(Camera* camera, const glm::mat4& projection);

private:
    GLuint _windowTexture = 0;

    Shader*           _windowShader = NULL;
    WindowVertexData* _windowVertexData = NULL;
};

class FrameBufferScene : public DepthScene2
{
public:
    FrameBufferScene(QOpenGLFunctions_3_3_Core* glCore);
    ~FrameBufferScene();

    void paint(Camera* camera, const glm::mat4& projection);

private:
    GLuint _FBO;
    GLuint _colorBuf;
    GLuint _RBO;

    Shader*         _quadShader = NULL;
    QuadVertexData* _quadVertexData = NULL;
};

class MainWindow : public QOpenGLWindow, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void focusInEvent(QFocusEvent *) override;

private:
    void drawMoel();

private:
    void resetCursor();

private:
    glm::mat4 _projection;
    Camera*   _camera = NULL;

    const char* _modelVsFile= "model.vs";
    const char* _modelFsFile= "model.fs";
    Shader*     _modelShader = NULL;
    Model*      _model = NULL;

    DepthScene* _depthScene = NULL;

    ObjectOutlingScene* _objectOutlingScene = NULL;

    Blending1Scene* _blending1Scene = NULL;

    Blending2Scene* _blending2Scene = NULL;

    FrameBufferScene* _frameBufferScene = NULL;
};
#endif // MAINWINDOW_H
