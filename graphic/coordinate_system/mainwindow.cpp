#include "mainwindow.h"

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
}

// world space positions of our cubes
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

void MainWindow::initializeGL()
{
    resize(QSize(800, 800));

    _glcore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

    _glcore->glEnable(GL_DEPTH_TEST);

    for (int i=0; i<10; i++)
    {
        _squares.push_back(std::shared_ptr<Square>(new Square(_glcore)));
    }

    for (auto& ele : _squares)
    {
        ele->init();
    }

    _view = glm::mat4(1.0f);
    _view = glm::translate(_view, glm::vec3(0.0f, 0.0f, -5.0f));
    _projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}

void MainWindow::resizeGL(int w, int h)
{
    _glcore->glViewport(0, 0, w, h);
}

void MainWindow::paintGL()
{
    // clear background
    _glcore->glClearColor(0.2f, 0.3f, 0.3f, 1.0);
    _glcore->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int i = 0;
    for (auto& ele : _squares)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        i++;

        ele->setView(_view);
        ele->setProjection(_projection);
        ele->setModel(model);

        ele->paint();
    }
}
