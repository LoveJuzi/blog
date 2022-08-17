#include "mainwindow.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <QTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDateTime>

MainWindow::MainWindow()
{
    resize(800, 800);
}

MainWindow::~MainWindow()
{
    _SAFE_DEL(_triangle);
    _SAFE_DEL(_triangleShader);

    _SAFE_DEL(_triangle2);
    _SAFE_DEL(_triangle2Shader);

    _SAFE_DEL(_wallTexture);

    _SAFE_DEL(_triangle3);
    _SAFE_DEL(_triangle3Shader);

    _SAFE_DEL(_awesomefaceTexture);

    _SAFE_DEL(_triangle4);
    _SAFE_DEL(_triangle4Shader);
    _SAFE_DEL(_triangle4Trans);

    _SAFE_DEL(_triangle5);
    _SAFE_DEL(_triangle5Shader);
    _SAFE_DEL(_triangle5Model);
    _SAFE_DEL(_triangle5View);
    _SAFE_DEL(_triangle5Projection);

    _SAFE_DEL(_cube);
    _SAFE_DEL(_cubeShader);
    _SAFE_DEL(_cubeModel);
    _SAFE_DEL(_cubeView);
    _SAFE_DEL(_cubeProjection);

    _SAFE_DEL(_camera);

    _SAFE_DEL(_light);
    _SAFE_DEL(_lightShader);
    _SAFE_DEL(_lightModel);
    _SAFE_DEL(_lightProjection);

    _SAFE_DEL(_box);
    _SAFE_DEL(_boxShader);
    _SAFE_DEL(_boxModel);

    _SAFE_DEL(_containerTexture);
    _SAFE_DEL(_containerSpecularTexture);

    _SAFE_DEL(_box2Shader);
    _SAFE_DEL(_box2);
    _SAFE_DEL(_box2Model);

    _SAFE_DEL(_directLight);
    _SAFE_DEL(_directLightShader);
    _SAFE_DEL(_directLightBox);
    _SAFE_DEL(_directLightBoxModel);

    _SAFE_DEL(_pointLight);
    _SAFE_DEL(_pointLightShapeShader);
    _SAFE_DEL(_pointLightShape);
    _SAFE_DEL(_pointLightShader);
    _SAFE_DEL(_pointLightBox);

    _SAFE_DEL(_spotLight);
    _SAFE_DEL(_spotLightShader);
    _SAFE_DEL(_spotLightBox);

    _SAFE_DEL(_multiLightShader);
    _SAFE_DEL(_multiLightBox);
}

void MainWindow::initializeGL()
{
    if (!initializeOpenGLFunctions())
    {
        std::cerr << "initializeOpenGLFunctions failed!" << std::endl;
    }

    _wallTexture = new TextureData(this);
    _wallTexture->init(_wallImageFile);

    _awesomefaceTexture = new PngTextureData(this);
    _awesomefaceTexture->init(_awesomefacePngFile);

    _triangleShader = new Shader(this);
    _triangleShader->init(_triangleVsFile, _triangleFsFile);
    _triangle = new Triangle(this, _triangleShader);
    _triangle->init();

    _triangle2Shader = new Shader(this);
    _triangle2Shader->init(_triangle2VsFile, _triangle2FsFile);
    _triangle2 = new Triangle2(this);
    _triangle2->init(_triangle2Shader, _wallTexture->getTexture());

    _triangle3Shader = new Shader(this);
    _triangle3Shader->init(_triangle3VsFile, _triangle3FsFile);
    _triangle3 = new Triangle3(this);
    _triangle3->init(_triangle3Shader, _wallTexture->getTexture(),
                     _awesomefaceTexture->getTexture());

    _triangle4Trans = new glm::mat4(1.0f);
    *_triangle4Trans = glm::rotate(*_triangle4Trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    *_triangle4Trans = glm::scale(*_triangle4Trans, glm::vec3(0.5, 0.5, 0.5));

    _triangle4Shader = new Shader(this);
    _triangle4Shader->init(_triangle4VsFile, _triangle4FsFile);
    _triangle4 = new Triangle4(this);
    _triangle4->init(_triangle4Shader, _wallTexture->getTexture(),
                     _awesomefaceTexture->getTexture(), _triangle4Trans);

    _triangle5Model = new glm::mat4(1.0f);
    _triangle5View = new glm::mat4(1.0f);
    _triangle5Projection = new glm::mat4(1.0f);
    *_triangle5Model = glm::rotate(*_triangle5Model, glm::radians(-55.0f),
                                   glm::vec3(1.0f, 0.0f, 0.0f));
    *_triangle5View = glm::translate(*_triangle5View, glm::vec3(0.0f, 0.0f, -3.0f));
    _triangle5Shader = new Shader(this);
    _triangle5Shader->init(_triangle5VsFile, _triangle5FsFile);
    _triangle5 = new Triangle5(this);
    _triangle5->init(_triangle5Shader, _wallTexture->getTexture(),
                     _awesomefaceTexture->getTexture(),
                     _triangle5Model, _triangle5View,
                     _triangle5Projection);

    _cubeModel = new glm::mat4(1.0f);
    _cubeView = new glm::mat4(1.0f);
    _cubeProjection = new glm::mat4(1.0f);

    _cubeShader = new Shader(this);
    _cubeShader->init(_cubeVsFile, _cubeFsFile);
    _cube = new Cube(this);
    _cube->init(_cubeShader, _wallTexture->getTexture(),
                _awesomefaceTexture->getTexture());

    _camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

    _lightModel = new glm::mat4(1.0f);
    _lightProjection = new glm::mat4(1.0f);
    _lightShader = new Shader(this);
    _lightShader->init(_lightVsFile, _lightFsFile);
    _light = new Light(this);
    _light->init(_lightShader);

    _boxModel = new glm::mat4(1.0f);
    _boxShader = new Shader(this);
    _boxShader->init(_boxVsFile, _boxFsFile);
    _box = new Box(this);
    _box->init(_boxShader);

    _containerTexture = new PngTextureData(this);
    _containerTexture->init(_containerPngFile);

    _containerSpecularTexture = new PngTextureData(this);
    _containerSpecularTexture->init(_containerSpecularPngFile);

    _box2Shader = new Shader(this);
    _box2Shader->init(_box2VsFile, _box2FsFile);
    _box2 = new Box2(this);
    _box2->init();

    _directLight = new DirectLight();
    _directLightShader = new Shader(this);
    _directLightShader->init(_directLightVsFile, _directLightFsFile);
    _directLightBox = new Box2(this);
    _directLightBox->init();
    _directLightBoxModel = new glm::mat4(1.0f);

    _pointLight = new PointLight();
    _pointLightShapeShader = new Shader(this);
    _pointLightShapeShader->init(_pointLightShapeVsFile, _pointLightShapeFsFile);
    _pointLightShape = new PointLightShape(this);
    _pointLightShape->init();

    _pointLightShader = new Shader(this);
    _pointLightShader->init(_pointLightVsFile, _pointLightFsFile);
    _pointLightBox = new Box2(this);
    _pointLightBox->init();

    _spotLight = new SpotLight();
    _spotLightShader = new Shader(this);
    _spotLightShader->init(_spotLightVsFile, _spotLightFsFile);
    _spotLightBox = new Box2(this);
    _spotLightBox->init();

    _multiLightShader = new Shader(this);
    _multiLightShader->init(_multiLightVsFile, _multiLightFsFile);
    _multiLightBox = new Box2(this);
    _multiLightBox->init();

    glEnable(GL_DEPTH_TEST);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&](){
        update();
    });
    timer->start(30);
}

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    *_triangle5Projection = glm::perspective(glm::radians(45.0f),
                                             (float)(w/h), 0.1f, 100.0f);
    *_cubeProjection = glm::perspective(glm::radians(45.0f),
                                        (float)(h/h), 0.1f, 100.0f);
    *_lightProjection = glm::perspective(glm::radians(45.0f),
                                        (float)(h/h), 0.1f, 100.0f);

    resetCursor();

    update();  // call opengl redraw
}

void MainWindow::paintGL()
{
    // trianglePaint();
    // cubePaint();
    // lightPaint();
    multiBoxPaint();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    auto deltaTime = [&]() -> GLfloat {
        QDateTime currentTime = QDateTime::currentDateTime();
        static QDateTime lastTime = currentTime.addMSecs(-30);
        defer(lastTime = currentTime);
        // std::cout << lastTime.msecsTo(currentTime) << std::endl;
        if (lastTime.msecsTo(currentTime) > 100) return 30 / 1000.0f;
        return lastTime.msecsTo(currentTime) / 1000.0f;
    };

    switch (e->key())
    {
    case Qt::Key_W:
        // std::cout << "W" << std::endl;
        _camera->processKeyboard(Camera::Movement::FORWARD, deltaTime());
        break;
    case Qt::Key_S:
        //std::cout << "S" << std::endl;
        _camera->processKeyboard(Camera::Movement::BACKWARD, deltaTime());
        break;
    case Qt::Key_A:
        //std::cout << "A" << std::endl;
        _camera->processKeyboard(Camera::Movement::LEFT, deltaTime());
        break;
    case Qt::Key_D:
        //std::cout << "D" << std::endl;
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

void MainWindow::trianglePaint()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // _triangle->draw();
    // _triangle2->draw();
    // _triangle3->draw();
    // _triangle4->draw();
    _triangle5->draw();
}

void MainWindow::cubePaint()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto drawTenCube = [&]() {
        glm::vec3 cubePositions[] = {
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

        *_cubeView = _camera->getViewMatrix();
        for (int i = 0 ; i < 10; ++i) {
            float angle = 20.0 * i;
            *_cubeModel = glm::mat4(1.0f);
            *_cubeModel = glm::translate(*_cubeModel, cubePositions[i]);
            *_cubeModel = glm::rotate(*_cubeModel,
                                      glm::radians(angle),
                                      glm::vec3(0.5f, 1.0f, 0.0f));
            _cube->draw(*_cubeModel, *_cubeView, *_cubeProjection);
        }
    };
    drawTenCube();

    auto drawTenCube2 = [&]() {
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
        static GLfloat radius = 10.0f;
        static GLfloat frameAngle = 0.0f;
        GLfloat camX = sin(frameAngle) * radius;
        GLfloat camZ = cos(frameAngle) * radius;
        frameAngle -= 0.01;
        *_cubeView = glm::lookAt(glm::vec3(camX, 0.0, camZ),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0 ; i < 10; ++i) {
            float angle = 20.0 * i;
            *_cubeModel = glm::mat4(1.0f);
            *_cubeModel = glm::translate(*_cubeModel, cubePositions[i]);
            *_cubeModel = glm::rotate(*_cubeModel,
                                      glm::radians(angle),
                                      glm::vec3(0.5f, 1.0f, 0.0f));

            _cube->draw(*_cubeModel, *_cubeView, *_cubeProjection);
        }
    };
    // drawTenCube2();
}

void MainWindow::lightPaint()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    *_lightModel = glm::mat4(1.0f);
    *_lightModel = glm::translate(*_lightModel, _light->getLightPos());
    _light->draw(*_lightModel, _camera->getViewMatrix(), *_lightProjection);

    auto drawBox = [&] () {
        *_boxModel = glm::mat4(1.0f);
        _boxShader->use();
        _boxShader->setVec3("light.position", _light->getLightPos());
        _boxShader->setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        _boxShader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        _boxShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        _boxShader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        _boxShader->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        _boxShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        _boxShader->setFloat("material.shininess", 32.0f);
        _boxShader->setVec3("viewPos", _camera->getPosition());
        _boxShader->setMat4("model", 1, GL_FALSE, *_boxModel);
        _boxShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _boxShader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        _box->draw();
    };
    // drawBox();

    auto drawBox2 = [&]() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _containerTexture->getTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _containerSpecularTexture->getTexture());
        *_boxModel = glm::mat4(1.0f);

        _box2Shader->use();
        _box2Shader->setVec3("light.position", _light->getLightPos());
        _box2Shader->setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        _box2Shader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        _box2Shader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        _box2Shader->setInt("material.diffuse", 0);
        _box2Shader->setInt("material.specular", 1);
        _box2Shader->setFloat("material.shininess", 32.0f);
        _box2Shader->setVec3("viewPos", _camera->getPosition());
        _box2Shader->setMat4("model", 1, GL_FALSE, *_boxModel);
        _box2Shader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _box2Shader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        _box2->draw();
    };
    drawBox2();
}

void MainWindow::multiBoxPaint()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 boxPosition[] = {
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

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _containerTexture->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _containerSpecularTexture->getTexture());

    auto drawMultiLight = [&]() {
        _pointLightShapeShader->use();
        _pointLightShapeShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _pointLightShapeShader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        _pointLightShapeShader->setVec3("lightColor", _pointLight->getSpecular());
        for (int i=0; i<4; ++i) {
            glm::mat4 shapeModel(1.0f);
            shapeModel = glm::translate(shapeModel, pointLightPositions[i]);
            shapeModel = glm::scale(shapeModel, glm::vec3(0.2f, 0.2f, 0.2f));

            _pointLightShapeShader->setMat4("model", 1, GL_FALSE, shapeModel);
            _pointLightShape->draw();
        }

        _spotLight->setPosition(_camera->getPosition());
        _spotLight->setDirection(_camera->getFront());

        _multiLightShader->use();

        _multiLightShader->setVec3("viewPos", _camera->getPosition());
        _multiLightShader->setVec3("dirLight.direction", _directLight->getDirection());
        _multiLightShader->setVec3("dirLight.ambient", _directLight->getAmbient());
        _multiLightShader->setVec3("dirLight.diffuse", _directLight->getDiffuse());
        _multiLightShader->setVec3("dirLight.specular", _directLight->getSpecular());

        _multiLightShader->setVec3("pointLights[0].position", pointLightPositions[0]);
        _multiLightShader->setVec3("pointLights[0].ambient", _pointLight->getAmbient());
        _multiLightShader->setVec3("pointLights[0].diffuse", _pointLight->getDiffuse());
        _multiLightShader->setVec3("pointLights[0].specular", _pointLight->getSpecular());
        _multiLightShader->setFloat("pointLights[0].constant", _pointLight->getConstant());
        _multiLightShader->setFloat("pointLights[0].linear", _pointLight->getLinear());
        _multiLightShader->setFloat("pointLights[0].quadratic", _pointLight->getQuadratic());

        _multiLightShader->setVec3("pointLights[1].position", pointLightPositions[1]);
        _multiLightShader->setVec3("pointLights[1].ambient", _pointLight->getAmbient());
        _multiLightShader->setVec3("pointLights[1].diffuse", _pointLight->getDiffuse());
        _multiLightShader->setVec3("pointLights[1].specular", _pointLight->getSpecular());
        _multiLightShader->setFloat("pointLights[1].constant", _pointLight->getConstant());
        _multiLightShader->setFloat("pointLights[1].linear", _pointLight->getLinear());
        _multiLightShader->setFloat("pointLights[1].quadratic", _pointLight->getQuadratic());

        _multiLightShader->setVec3("pointLights[2].position", pointLightPositions[2]);
        _multiLightShader->setVec3("pointLights[2].ambient", _pointLight->getAmbient());
        _multiLightShader->setVec3("pointLights[2].diffuse", _pointLight->getDiffuse());
        _multiLightShader->setVec3("pointLights[2].specular", _pointLight->getSpecular());
        _multiLightShader->setFloat("pointLights[2].constant", _pointLight->getConstant());
        _multiLightShader->setFloat("pointLights[2].linear", _pointLight->getLinear());
        _multiLightShader->setFloat("pointLights[2].quadratic", _pointLight->getQuadratic());

        _multiLightShader->setVec3("pointLights[3].position", pointLightPositions[3]);
        _multiLightShader->setVec3("pointLights[3].ambient", _pointLight->getAmbient());
        _multiLightShader->setVec3("pointLights[3].diffuse", _pointLight->getDiffuse());
        _multiLightShader->setVec3("pointLights[3].specular", _pointLight->getSpecular());
        _multiLightShader->setFloat("pointLights[3].constant", _pointLight->getConstant());
        _multiLightShader->setFloat("pointLights[3].linear", _pointLight->getLinear());
        _multiLightShader->setFloat("pointLights[3].quadratic", _pointLight->getQuadratic());

        _multiLightShader->setVec3("spotLight.position", _spotLight->getPosition());
        _multiLightShader->setVec3("spotLight.direction", _spotLight->getDirection());
        _multiLightShader->setFloat("spotLight.cutOff", _spotLight->getCutOff());
        _multiLightShader->setFloat("spotLight.outerCutOff", _spotLight->getOuterCutOff());
        _multiLightShader->setVec3("spotLight.ambient", _spotLight->getAmbient());
        _multiLightShader->setVec3("spotLight.diffuse", _spotLight->getDiffuse());
        _multiLightShader->setVec3("spotLight.specular", _spotLight->getSpecular());
        _multiLightShader->setFloat("spotLight.constant", _spotLight->getConstant());
        _multiLightShader->setFloat("spotLight.linear", _spotLight->getLinear());
        _multiLightShader->setFloat("spotLight.quadratic", _spotLight->getQuadratic());

        _multiLightShader->setInt("material.diffuse", 0);
        _multiLightShader->setInt("material.specular", 1);
        _multiLightShader->setFloat("material.shininess", 32.0f);
        _multiLightShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _multiLightShader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        for (int i = 0 ; i < 10; ++i)
        {
            float angle = 20.0 * i;
            glm::mat4 model(1.0f);
            model = glm::translate(model, boxPosition[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

            _multiLightShader->setMat4("model", 1, GL_FALSE, model);
            _multiLightBox->draw();
        }

    };
    drawMultiLight();

    auto drawDirectLight = [&]() {
        _directLightShader->use();
        _directLightShader->setVec3("directLight.direction", _directLight->getDirection());
        _directLightShader->setVec3("directLight.ambient", _directLight->getAmbient());
        _directLightShader->setVec3("directLight.diffuse", _directLight->getDiffuse());
        _directLightShader->setVec3("directLight.specular", _directLight->getSpecular());
        _directLightShader->setInt("material.diffuse", 0);
        _directLightShader->setInt("material.specular", 1);
        _directLightShader->setFloat("material.shininess", 32.0f);
        _directLightShader->setVec3("viewPos", _camera->getPosition());
        _directLightShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _directLightShader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        for (int i = 0 ; i < 10; ++i)
        {
            float angle = 20.0 * i;
            *_directLightBoxModel = glm::mat4(1.0f);
            *_directLightBoxModel = glm::translate(*_directLightBoxModel, boxPosition[i]);
            *_directLightBoxModel = glm::rotate(*_directLightBoxModel,
                                                glm::radians(angle),
                                                glm::vec3(0.5f, 1.0f, 0.0f));

            _directLightShader->setMat4("model", 1, GL_FALSE, *_directLightBoxModel);
            _directLightBox->draw();
        }
    };
    //drawDirectLight();

    auto drawPointLight = [&]() {
        glm::mat4 shapeModel(1.0f);
        shapeModel = glm::translate(shapeModel, _pointLight->getPosition());
        shapeModel = glm::scale(shapeModel, glm::vec3(0.2f, 0.2f, 0.2f));
        _pointLightShapeShader->use();

        _pointLightShapeShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _pointLightShapeShader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        _pointLightShapeShader->setMat4("model", 1, GL_FALSE, shapeModel);
        _pointLightShapeShader->setVec3("lightColor", _pointLight->getSpecular());
        _pointLightShape->draw();

        _pointLightShader->use();
        _pointLightShader->setVec3("pointLight.position", _pointLight->getPosition());
        _pointLightShader->setVec3("pointLight.ambient", _pointLight->getAmbient());
        _pointLightShader->setVec3("pointLight.diffuse", _pointLight->getDiffuse());
        _pointLightShader->setVec3("pointLight.specular", _pointLight->getSpecular());
        _pointLightShader->setFloat("pointLight.constant", _pointLight->getConstant());
        _pointLightShader->setFloat("pointLight.linear", _pointLight->getLinear());
        _pointLightShader->setFloat("pointLight.quadratic", _pointLight->getQuadratic());
        _pointLightShader->setInt("material.diffuse", 0);
        _pointLightShader->setInt("material.specular", 0);
        _pointLightShader->setFloat("material.shininess", 32.0f);
        _pointLightShader->setVec3("viewPos", _camera->getPosition());
        _pointLightShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _pointLightShader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        for (int i = 0 ; i < 10; ++i)
        {
            float angle = 20.0 * i;
            glm::mat4 model(1.0f);
            model = glm::translate(model, boxPosition[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

            _directLightShader->setMat4("model", 1, GL_FALSE, model);
            _directLightBox->draw();
        }
    };
    // drawPointLight();

    auto drawSpotLight = [&]() {
        _spotLight->setDirection(_camera->getFront());
        _spotLight->setPosition(_camera->getPosition());
        _spotLightShader->use();
        _spotLightShader->setVec3("spotLight.direction", _spotLight->getDirection());
        _spotLightShader->setVec3("spotLight.position", _spotLight->getPosition());
        _spotLightShader->setFloat("spotLight.cutOff", _spotLight->getCutOff());
        _spotLightShader->setFloat("spotLight.outerCutOff", _spotLight->getOuterCutOff());
        _spotLightShader->setVec3("spotLight.ambient", _spotLight->getAmbient());
        _spotLightShader->setVec3("spotLight.diffuse", _spotLight->getDiffuse());
        _spotLightShader->setVec3("spotLight.specular", _spotLight->getSpecular());
        _spotLightShader->setFloat("spotLight.constant", _spotLight->getConstant());
        _spotLightShader->setFloat("spotLight.linear", _spotLight->getLinear());
        _spotLightShader->setFloat("spotLight.quadratic", _spotLight->getQuadratic());
        _spotLightShader->setInt("material.diffuse", 0);
        _spotLightShader->setInt("material.specular", 1);
        _spotLightShader->setFloat("material.shininess", 32.0f);
        _spotLightShader->setVec3("viewPos", _camera->getPosition());
        _spotLightShader->setMat4("view", 1, GL_FALSE, _camera->getViewMatrix());
        _spotLightShader->setMat4("projection", 1, GL_FALSE, *_lightProjection);
        for (int i = 0 ; i < 10; ++i)
        {
            float angle = 20.0 * i;
            glm::mat4 model(1.0f);
            model = glm::translate(model, boxPosition[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

            _spotLightShader->setMat4("model", 1, GL_FALSE, model);
            _spotLightBox->draw();
        }

    };
    drawSpotLight();


}

MainWindow::TextureData::TextureData(QOpenGLFunctions_3_3_Core* glCore)
{
    _glCore = glCore;
    if (_glCore == NULL) return;
    _glCore->glGenTextures(1, &_texture);
}

MainWindow::TextureData::~TextureData()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteTextures(1, &_texture);
}

bool MainWindow::TextureData::init(const char* imgFile)
{
    if (_glCore == NULL) return false;

    int width = 0;
    int height = 0;
    int nrChannels = 0;
    unsigned char* data = stbi_load(imgFile, &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return false;
    }
    defer(stbi_image_free(data));

    _glCore->glBindTexture(GL_TEXTURE_2D, _texture);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    _glCore->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                          GL_UNSIGNED_BYTE, data);
    _glCore->glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

MainWindow::PngTextureData::PngTextureData(QOpenGLFunctions_3_3_Core* glCore)
{
    _glCore = glCore;
    if (_glCore == NULL) return;
    _glCore->glGenTextures(1, &_texture);
}

MainWindow::PngTextureData::~PngTextureData()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteTextures(1, &_texture);
}

bool MainWindow::PngTextureData::init(const char* imgFile)
{
    if (_glCore == NULL) return false;

    int width = 0;
    int height = 0;
    int nrChannels = 0;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    defer(stbi_set_flip_vertically_on_load(false));
    unsigned char* data = stbi_load(imgFile, &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return false;
    }
    defer(stbi_image_free(data));

    _glCore->glBindTexture(GL_TEXTURE_2D, _texture);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    _glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    _glCore->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                          GL_UNSIGNED_BYTE, data);
    _glCore->glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

MainWindow::Shader::Shader(QOpenGLFunctions_3_3_Core* glCore)
{
    _glCore = glCore;
    if (_glCore == NULL) return;
    _ID = _glCore->glCreateProgram();
}

MainWindow::Shader::~Shader()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteProgram(_ID);
}

bool MainWindow::Shader::init(const char* vertexPath, const char* fragmentPath)
{
    if (_glCore == NULL) return false;

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        defer(vShaderFile.close());
        defer(fShaderFile.close());

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ: "
                  << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    vertex = _glCore->glCreateShader(GL_VERTEX_SHADER);
    fragment = _glCore->glCreateShader(GL_FRAGMENT_SHADER);
    defer(_glCore->glDeleteShader(vertex));
    defer(_glCore->glDeleteShader(fragment));

    _glCore->glShaderSource(vertex, 1, &vShaderCode, NULL);
    _glCore->glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    _glCore->glShaderSource(fragment, 1, &fShaderCode, NULL);
    _glCore->glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    _glCore->glAttachShader(_ID, vertex);
    _glCore->glAttachShader(_ID, fragment);
    _glCore->glLinkProgram(_ID);
    checkCompileErrors(_ID, "PROGRAM");

    return true;
}

void MainWindow::Shader::use()
{
    _glCore->glUseProgram(_ID);
}

void MainWindow::Shader::setBool(const std::string& name, bool value) const
{
    _glCore->glUniform1i(_glCore->glGetUniformLocation(_ID, name.c_str()), (int)value);
}

void MainWindow::Shader::setInt(const std::string& name, int value) const
{
    _glCore->glUniform1i(_glCore->glGetUniformLocation(_ID, name.c_str()), value);
}

void MainWindow::Shader::setFloat(const std::string& name, float value) const
{
    _glCore->glUniform1f(_glCore->glGetUniformLocation(_ID, name.c_str()), value);
}

void MainWindow::Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    _glCore->glUniform3fv(_glCore->glGetUniformLocation(_ID, name.c_str()),
                          1, &value[0]);
}

void MainWindow::Shader::setMat4(const std::string& name, GLsizei count,
                                 GLboolean transpose, const glm::mat4& value) const
{
    _glCore->glUniformMatrix4fv(_glCore->glGetUniformLocation(_ID, name.c_str()),
                                count, transpose, glm::value_ptr(value));
}

void MainWindow::Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success = 0;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        _glCore->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            _glCore->glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: "
                      << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
    else
    {
        _glCore->glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            _glCore->glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: "
                      << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}

const GLfloat MainWindow::Camera::YAW         = 180.0f;
const GLfloat MainWindow::Camera::PITCH       = 0.0f;
const GLfloat MainWindow::Camera::SPEED       = 2.0f;
const GLfloat MainWindow::Camera::SENSITIVITY = 0.05f;
const GLfloat MainWindow::Camera::ZOOM        = 45.0f;
MainWindow::Camera::Camera(const glm::vec3& position,
                           const glm::vec3& worldUp,
                           GLfloat yaw, GLfloat pitch) :
    _position(position),
    _worldUp(worldUp),
    _yaw(yaw),
    _pitch(pitch)
{
    _front = glm::vec3(0.0f, 0.0f, -1.0f);
    _movementSpeed = Camera::SPEED;
    _mouseSensitivity = Camera::SENSITIVITY;
    _zoom = Camera::ZOOM;
    updateCameraVectors();
}

MainWindow::Camera::~Camera()
{

}

glm::mat4 MainWindow::Camera::getViewMatrix()
{
    return glm::lookAt(_position, _position + _front, _up);
}

void MainWindow::Camera::processKeyboard(Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = _movementSpeed;
    if (deltaTime < 1.0f && deltaTime > 0.0f)
    {
        velocity *= deltaTime;
    }
    switch (direction)
    {
    case Movement::FORWARD:
        _position += _front * velocity;
        break;
    case Movement::BACKWARD:
        _position -= _front * velocity;
        break;
    case Movement::LEFT:
        _position -= _right * velocity;
        break;
    case Movement::RIGHT:
        _position += _right * velocity;
        break;
    default:
        break;
    }
}

void MainWindow::Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset,
                                              GLboolean constrainPitch)
{
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    _yaw   += xoffset;
    _pitch += yoffset;

    if (constrainPitch)
    {
        if (_pitch > 89.0f)  _pitch = 89.0f;
        if (_pitch < -89.0f) _pitch = -89.0f;
    }

    updateCameraVectors();
}

void MainWindow::Camera::processMouseScroll(GLfloat yoffset)
{
    _zoom -= yoffset;
    if (_zoom < 1.0f)  _zoom = 1.0f;
    if (_zoom > 45.0f) _zoom = 45.0f;
}

void MainWindow::Camera::updateCameraVectors()
{
    // calculate the new front vector
    glm::vec3 front;
    front.x = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up    = glm::normalize(glm::cross(_right, _front));
}

MainWindow::Triangle::Triangle(QOpenGLFunctions_3_3_Core* glCore, Shader* shader) :
    _vertices({
              -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
              0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
              0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
              })
{
    _glCore = glCore;
    _shader = shader;

    if (_glCore == NULL) return ;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Triangle::~Triangle()
{
    if (_glCore == NULL) return ;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Triangle::init()
{
    if (_glCore == NULL) return false;        

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);

    _glCore->glBindVertexArray(0);

    return true;
}

void MainWindow::Triangle::draw()
{
    if (_glCore == NULL || _shader == NULL) return ;
    _shader->use();
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 3);
}

MainWindow::Triangle2::Triangle2(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
              // position         // color          // texture coords
              -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
              0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
              0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
              })
{
    _glCore = glCore;
    if (_glCore == NULL) return ;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Triangle2::~Triangle2()
{
    if (_glCore == NULL) return ;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Triangle2::init(Shader* shader, GLuint texture)
{
    _shader = shader;
    _texture = texture;

    if (_glCore == NULL) return false;
    if (_shader == NULL) return false;
    if (_texture == 0)   return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                   (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                   (void*)(6 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(2);

    _glCore->glBindVertexArray(0);

    return true;
}

void MainWindow::Triangle2::draw()
{
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture);

    _shader->use();
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 3);
}

MainWindow::Triangle3::Triangle3(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
              // position         // color          // texture0 coords  // texture1 coords
              -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,         0.0f, 0.0f,
              0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,         1.0f, 0.0f,
              0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f,         0.5f, 1.0f,
              })
{
    _glCore = glCore;
    if (_glCore == NULL) return ;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Triangle3::~Triangle3()
{
    if (_glCore == NULL) return ;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Triangle3::init(Shader* shader, GLuint texture0, GLuint texture1)
{
    _shader = shader;
    _texture0 = texture0;
    _texture1 = texture1;

    if (_glCore == NULL) return false;
    if (_shader == NULL) return false;
    if (_texture0 == 0)  return false;
    if (_texture1 == 0)  return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), 0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(6 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(2);
    _glCore->glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(8 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(3);

    _glCore->glBindVertexArray(0);

    _shader->use();
    _shader->setInt("texture0", 0);
    _shader->setInt("texture1", 1);

    return true;
}

void MainWindow::Triangle3::draw()
{
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture0);
    _glCore->glActiveTexture(GL_TEXTURE1);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture1);

    _shader->use();
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 3);
}

MainWindow::Triangle4::Triangle4(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
              // position         // color          // texture0 coords  // texture1 coords
              -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,         0.0f, 0.0f,
              0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,         1.0f, 0.0f,
              0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f,         0.5f, 1.0f,
              })
{
    _glCore = glCore;
    if (_glCore == NULL) return ;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Triangle4::~Triangle4()
{
    if (_glCore == NULL) return ;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Triangle4::init(Shader* shader, GLuint texture0, GLuint texture1,
                                 glm::mat4* trans)
{
    _shader = shader;
    _texture0 = texture0;
    _texture1 = texture1;
    _trans = trans;

    if (_glCore == NULL) return false;
    if (_shader == NULL) return false;
    if (_texture0 == 0)  return false;
    if (_texture1 == 0)  return false;
    if (_trans == 0)     return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), 0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(6 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(2);
    _glCore->glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(8 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(3);

    _glCore->glBindVertexArray(0);

    _shader->use();
    _shader->setInt("texture0", 0);
    _shader->setInt("texture1", 1);

    return true;
}

void MainWindow::Triangle4::draw()
{
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture0);
    _glCore->glActiveTexture(GL_TEXTURE1);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture1);

    _shader->use();
    _shader->setMat4("transform", 1, GL_FALSE, *_trans);

    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 3);
}

MainWindow::Triangle5::Triangle5(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
              // position         // color          // texture0 coords  // texture1 coords
              -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,         0.0f, 0.0f,
              0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,         1.0f, 0.0f,
              0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 1.0f,         0.5f, 1.0f,
              })
{
    _glCore = glCore;
    if (_glCore == NULL) return ;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Triangle5::~Triangle5()
{
    if (_glCore == NULL) return ;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Triangle5::init(Shader* shader, GLuint texture0, GLuint texture1,
                                 glm::mat4* model, glm::mat4* view, glm::mat4* projection)
{
    _shader = shader;
    _texture0 = texture0;
    _texture1 = texture1;
    _model = model;
    _view = view;
    _projection = projection;

    if (_glCore == NULL)     return false;
    if (_shader == NULL)     return false;
    if (_texture0 == 0)      return false;
    if (_texture1 == 0)      return false;
    if (_model == NULL)      return false;
    if (_view == NULL)       return false;
    if (_projection == NULL) return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), 0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(6 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(2);
    _glCore->glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat),
                                   (void*)(8 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(3);

    _glCore->glBindVertexArray(0);

    _shader->use();
    _shader->setInt("texture0", 0);
    _shader->setInt("texture1", 1);

    return true;
}

void MainWindow::Triangle5::draw()
{
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture0);
    _glCore->glActiveTexture(GL_TEXTURE1);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture1);

    _shader->use();
    _shader->setMat4("model", 1, GL_FALSE, *_model);
    _shader->setMat4("view", 1, GL_FALSE, *_view);
    _shader->setMat4("projection", 1, GL_FALSE, *_projection);
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, 3);
}

MainWindow::Cube::Cube(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
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
              })
{
    _glCore = glCore;
    if (_glCore == NULL) return;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Cube::~Cube()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Cube::init(Shader* shader, GLuint texture0, GLuint texture1)
{
    _shader = shader;
    _texture0 = texture0;
    _texture1 = texture1;

    if (_glCore == NULL) return false;
    if (_shader == NULL) return false;
    if (_texture0 == 0)  return false;
    if (_texture1 == 0)  return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                                   (void*)0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                                    (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glBindVertexArray(0);

    _shader->use();
    _shader->setInt("texture0", 0);
    _shader->setInt("texture1", 1);

    return true;
}

void MainWindow::Cube::draw(const glm::mat4& model,
                            const glm::mat4& view,
                            const glm::mat4& projection)
{
    _glCore->glActiveTexture(GL_TEXTURE0);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture0);
    _glCore->glActiveTexture(GL_TEXTURE1);
    _glCore->glBindTexture(GL_TEXTURE_2D, _texture1);

    _shader->use();
    _shader->setMat4("model", 1, GL_FALSE, model);
    _shader->setMat4("view", 1, GL_FALSE, view);
    _shader->setMat4("projection", 1, GL_FALSE, projection);
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}

MainWindow::Light::Light(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
              -0.5f, -0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f,  0.5f, -0.5f,
               0.5f,  0.5f, -0.5f,
              -0.5f,  0.5f, -0.5f,
              -0.5f, -0.5f, -0.5f,

              -0.5f, -0.5f,  0.5f,
               0.5f, -0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f,  0.5f,
              -0.5f, -0.5f,  0.5f,

              -0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f, -0.5f,
              -0.5f, -0.5f, -0.5f,
              -0.5f, -0.5f, -0.5f,
              -0.5f, -0.5f,  0.5f,
              -0.5f,  0.5f,  0.5f,

               0.5f,  0.5f,  0.5f,
               0.5f,  0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f, -0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,

              -0.5f, -0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f, -0.5f,  0.5f,
               0.5f, -0.5f,  0.5f,
              -0.5f, -0.5f,  0.5f,
              -0.5f, -0.5f, -0.5f,

              -0.5f,  0.5f, -0.5f,
               0.5f,  0.5f, -0.5f,
               0.5f,  0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f, -0.5f,
              })
{
    _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    _lightPos = glm::vec3(6.0f, 5.0f, 10.0f);
    _glCore = glCore;
    if (_glCore == NULL) return;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Light::~Light()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Light::init(Shader* shader)
{
    _shader = shader;

    if (_glCore == NULL) return false;
    if (_shader == NULL) return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                                   (void*)0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glBindVertexArray(0);

    return true;
}

void MainWindow::Light::draw(const glm::mat4& model,
                             const glm::mat4& view,
                             const glm::mat4& projection)
{
    _shader->use();
    _shader->setVec3("lightColor", _lightColor);
    _shader->setMat4("model", 1, GL_FALSE, model);
    _shader->setMat4("scale", 1, GL_FALSE,
                     glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
    _shader->setMat4("view", 1, GL_FALSE, view);
    _shader->setMat4("projection", 1, GL_FALSE, projection);
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}

MainWindow::PointLightShape::PointLightShape(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
              -0.5f, -0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f,  0.5f, -0.5f,
               0.5f,  0.5f, -0.5f,
              -0.5f,  0.5f, -0.5f,
              -0.5f, -0.5f, -0.5f,

              -0.5f, -0.5f,  0.5f,
               0.5f, -0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f,  0.5f,
              -0.5f, -0.5f,  0.5f,

              -0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f, -0.5f,
              -0.5f, -0.5f, -0.5f,
              -0.5f, -0.5f, -0.5f,
              -0.5f, -0.5f,  0.5f,
              -0.5f,  0.5f,  0.5f,

               0.5f,  0.5f,  0.5f,
               0.5f,  0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f, -0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,

              -0.5f, -0.5f, -0.5f,
               0.5f, -0.5f, -0.5f,
               0.5f, -0.5f,  0.5f,
               0.5f, -0.5f,  0.5f,
              -0.5f, -0.5f,  0.5f,
              -0.5f, -0.5f, -0.5f,

              -0.5f,  0.5f, -0.5f,
               0.5f,  0.5f, -0.5f,
               0.5f,  0.5f,  0.5f,
               0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f,  0.5f,
              -0.5f,  0.5f, -0.5f,
              })
{
    _glCore = glCore;
    if (_glCore == NULL) return;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::PointLightShape::~PointLightShape()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::PointLightShape::init()
{
    if (_glCore == NULL) return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                                   (void*)0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glBindVertexArray(0);

    return true;
}

void MainWindow::PointLightShape::draw()
{
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}

MainWindow::DirectLight::DirectLight() :
    _direction(-0.2f, -1.0f, -0.3f),
    _ambient(0.2f, 0.2f, 0.2f),
    _diffuse(0.5f, 0.5f, 0.5f),
    _specular(1.0f, 1.0f, 1.0f)
{

}

MainWindow::DirectLight::~DirectLight()
{

}

MainWindow::PointLight::PointLight() :
    _position(1.0f, 1.0f, 1.0f),
    _ambient(0.2f, 0.2f, 0.2f),
    _diffuse(0.5, 0.5f, 0.5f),
    _specular(1.0f, 1.0f, 1.0f),
    _constant(1.0f), _linear(0.09f), _quadratic(0.032f)
{

}

MainWindow::PointLight::~PointLight()
{

}

MainWindow::SpotLight::SpotLight() :
    _cutOff(glm::cos(glm::radians(12.5f))),
    _outerCutOff(glm::cos(glm::radians(17.5f))),
    _ambient(0.2f, 0.2f, 0.2f),
    _diffuse(0.8, 0.8f, 0.8f),
    _specular(1.0f, 1.0f, 1.0f),
    _constant(1.0f), _linear(0.09f), _quadratic(0.032f)
{

}

MainWindow::SpotLight::~SpotLight()
{

}

MainWindow::Box::Box(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
              -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
               0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
               0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
               0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
              -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
              -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

              -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
               0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
              -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
              -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

              -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
              -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
              -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
              -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
              -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
              -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

               0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
               0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
               0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
               0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
               0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
               0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

              -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
               0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
               0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
               0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
              -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
              -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

              -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
               0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
              -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
              -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
              })
{
    _boxColor = glm::vec3(1.0f, 0.5f, 0.31f);
    _glCore = glCore;
    if (_glCore == NULL) return;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Box::~Box()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Box::init(Shader* shader)
{
    _shader = shader;

    if (_glCore == NULL) return false;
    if (_shader == NULL) return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   (void*)0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glBindVertexArray(0);

    return true;
}

void MainWindow::Box::draw()
{
    _shader->use();
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}

MainWindow::Box2::Box2(QOpenGLFunctions_3_3_Core* glCore) :
    _vertices({
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
              })
{
    _boxColor = glm::vec3(1.0f, 0.5f, 0.31f);
    _glCore = glCore;
    if (_glCore == NULL) return;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
}

MainWindow::Box2::~Box2()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteVertexArrays(1, &_VAO);
    _glCore->glDeleteBuffers(1, &_VBO);
}

bool MainWindow::Box2::init()
{
    if (_glCore == NULL) return false;

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertices.size(),
                          &(_vertices[0]), GL_STATIC_DRAW);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                   (void*)0);
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                   (void*)(3 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                   (void*)(6 * sizeof(GLfloat)));
    _glCore->glEnableVertexAttribArray(2);
    _glCore->glBindVertexArray(0);

    return true;
}

void MainWindow::Box2::draw()
{
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}



