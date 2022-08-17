#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <map>

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

// define defer
template<typename Function>
struct Deferer{
    Function _f;

    explicit Deferer(Function f) : _f(f) {}
    ~Deferer()          { _f(); }
};

template<typename Function>
Deferer<Function> deferFunc(Function f) {
    return Deferer<Function>(f);
}

#define __DEFER_1__(x, y)  x##y
#define __DEFER_2__(x, y)  __DEFER_1__(x, y)
#define __DEFER_3__(x)     __DEFER_2__(x, __COUNTER__)
#define defer(expr)      auto __DEFER_3__(_defered_option_) = deferFunc([&](){expr;})

#ifndef _SAFE_DEL
#define _SAFE_DEL(x) \
  {if(x) {delete x; } x = NULL;}
class _STL_DELETE_OPERATOR_
{
  public:
    template < class _T>
    void operator () ( _T* pPtr){
      _SAFE_DEL(pPtr);
    }
    template < class key, class _T>
    void operator () ( std::pair<key, _T>& pPtr){
      _SAFE_DEL(pPtr.second);
    }
};
#endif

#ifndef _SAFE_CLEAR_STL_PTR_
#define _SAFE_CLEAR_STL_PTR_(x) \
{for_each(x.begin(),x.end(),icbase::_STL_DELETE_OPERATOR_()); \
    x.clear();}
#endif

#ifndef _DISABLE_COPY_
#define _DISABLE_COPY_(x) \
  private:\
    x(const x&);\
    x& operator =(const x&);
#endif

class MainWindow : public QOpenGLWindow, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:    
    class TextureData
    {
    public:
        TextureData(QOpenGLFunctions_3_3_Core* glCore);
        ~TextureData();
        bool init(const char* imgFile);
        GLuint getTexture() const { return _texture; }
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        GLuint _texture;
    };

    class PngTextureData
    {
    public:
        PngTextureData(QOpenGLFunctions_3_3_Core* glCore);
        ~PngTextureData();
        bool init(const char* imgFile);
        GLuint getTexture() const { return _texture; }
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        GLuint _texture;
    };

    class Shader
    {
    public:
        Shader(QOpenGLFunctions_3_3_Core* glCore);
        ~Shader();
        bool init(const char* vertexPath, const char* fragmentPath);
        void use();
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setMat4(const std::string& name, GLsizei count, GLboolean transpose,
                     const glm::mat4& value) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);

    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        unsigned int _ID;
    };

    class Camera
    {
    public:
        enum class Movement {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
        };

        static const GLfloat YAW;
        static const GLfloat PITCH;
        static const GLfloat SPEED;
        static const GLfloat SENSITIVITY;
        static const GLfloat ZOOM;

    public:
        Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
               const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
               GLfloat yaw   = Camera::YAW,
               GLfloat pitch = Camera::PITCH);
        ~Camera();

        glm::mat4 getViewMatrix();
        const glm::vec3& getPosition() const { return _position; }
        const glm::vec3& getFront() const { return _front; }
        void processKeyboard(Movement direction, GLfloat deltaTime);
        void processMouseMovement(GLfloat xoffset, GLfloat yoffset,
                                  GLboolean constrainPitch = true);
        void processMouseScroll(GLfloat yoffset);

    private:
        void updateCameraVectors();

    private:
        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _worldUp;

        GLfloat _yaw;
        GLfloat _pitch;

        GLfloat _movementSpeed;
        GLfloat _mouseSensitivity;
        GLfloat _zoom;
    };

    class Triangle
    {
    public:
        Triangle(QOpenGLFunctions_3_3_Core* glCore, Shader* shader);
        ~Triangle();
        bool init();
        void draw();
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        GLuint _VAO;
        GLuint _VBO;

        const std::vector<GLfloat> _vertices;
    };

    class Triangle2
    {
    public:
        Triangle2(QOpenGLFunctions_3_3_Core* glCore);
        ~Triangle2();
        bool init(Shader* shader, GLuint texture);
        void draw();
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        GLuint _texture = 0;
        GLuint _VAO;
        GLuint _VBO;

        const std::vector<GLfloat> _vertices;
    };

    class Triangle3
    {
    public:
        Triangle3(QOpenGLFunctions_3_3_Core* glCore);
        ~Triangle3();
        bool init(Shader* shader, GLuint texture0, GLuint texture1);
        void draw();
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        GLuint _texture0 = 0;
        GLuint _texture1 = 0;
        GLuint _VAO;
        GLuint _VBO;

        const std::vector<GLfloat> _vertices;
    };

    class Triangle4
    {
    public:
        Triangle4(QOpenGLFunctions_3_3_Core* glCore);
        ~Triangle4();
        bool init(Shader* shader, GLuint texture0, GLuint texture1, glm::mat4* trans);
        void draw();
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        glm::mat4* _trans;
        GLuint _texture0 = 0;
        GLuint _texture1 = 0;
        GLuint _VAO;
        GLuint _VBO;

        const std::vector<GLfloat> _vertices;
    };

    class Triangle5
    {
    public:
        Triangle5(QOpenGLFunctions_3_3_Core* glCore);
        ~Triangle5();
        bool init(Shader* shader, GLuint texture0, GLuint texture1,
                  glm::mat4* model, glm::mat4* view, glm::mat4* projection);
        void draw();
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        glm::mat4* _model = NULL;
        glm::mat4* _view = NULL;
        glm::mat4* _projection = NULL;
        GLuint _texture0 = 0;
        GLuint _texture1 = 0;
        GLuint _VAO;
        GLuint _VBO;

        const std::vector<GLfloat> _vertices;
    };

    class Cube
    {
    public:
        Cube(QOpenGLFunctions_3_3_Core* glCore);
        ~Cube();
        bool init(Shader* shader, GLuint texture0, GLuint texture1);
        void draw(const glm::mat4& mode,
                  const glm::mat4& view,
                  const glm::mat4& projection);
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        GLuint _VAO = 0;
        GLuint _VBO = 0;
        GLuint _texture0 = 0;
        GLuint _texture1 = 0;


        const std::vector<GLfloat> _vertices;
    };

    class Light
    {
    public:
        Light(QOpenGLFunctions_3_3_Core* glCore);
        ~Light();
        bool init(Shader* shader);
        const glm::vec3& getLightColor() const { return _lightColor; }
        const glm::vec3& getLightPos() const { return _lightPos; }
        void draw(const glm::mat4& mode,
                  const glm::mat4& view,
                  const glm::mat4& projection);
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        GLuint _VAO = 0;
        GLuint _VBO = 0;

        glm::vec3 _lightColor;
        glm::vec3 _lightPos;

        const std::vector<GLfloat> _vertices;
    };

    class PointLightShape
    {
    public:
        PointLightShape(QOpenGLFunctions_3_3_Core* glCore);
        ~PointLightShape();
        bool init();
        void draw();

    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        GLuint _VAO = 0;
        GLuint _VBO = 0;

        const std::vector<GLfloat> _vertices;
    };

    class DirectLight
    {
    public:
        DirectLight();
        ~DirectLight();

        const glm::vec3& getDirection() const { return _direction; }
        const glm::vec3& getAmbient() const { return _ambient; }
        const glm::vec3& getDiffuse() const { return _diffuse; }
        const glm::vec3& getSpecular() const { return _specular; }

    private:
        glm::vec3 _direction;
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
    };

    class PointLight
    {
    public:
        PointLight();
        ~PointLight();

        void setPositioin(const glm::vec3& v) { _position = v; }

        const glm::vec3& getPosition() const { return _position; }
        const glm::vec3& getAmbient() const { return _ambient; }
        const glm::vec3& getDiffuse() const { return _diffuse; }
        const glm::vec3& getSpecular() const { return _specular; }
        GLfloat getConstant() const { return _constant; }
        GLfloat getLinear() const { return _linear; }
        GLfloat getQuadratic() const { return _quadratic; }

    private:
        glm::vec3 _position;
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        GLfloat   _constant;
        GLfloat   _linear;
        GLfloat   _quadratic;
    };

    class SpotLight
    {
    public:
        SpotLight();
        ~SpotLight();

        void setDirection(const glm::vec3& d) { _direction = d; }
        void setPosition(const glm::vec3& p) { _position = p; }

        const glm::vec3& getDirection() const { return _direction; }
        const glm::vec3& getPosition() const { return _position; }
        GLfloat getCutOff() const { return _cutOff; }
        GLfloat getOuterCutOff() const { return _outerCutOff; }
        const glm::vec3& getAmbient() const { return _ambient; }
        const glm::vec3& getDiffuse() const { return _diffuse; }
        const glm::vec3& getSpecular() const { return _specular; }
        GLfloat getConstant() const { return _constant; }
        GLfloat getLinear() const { return _linear; }
        GLfloat getQuadratic() const { return _quadratic; }

    private:
        glm::vec3 _direction;
        glm::vec3 _position;
        GLfloat   _cutOff;
        GLfloat   _outerCutOff;
        glm::vec3 _ambient;
        glm::vec3 _diffuse;
        glm::vec3 _specular;
        GLfloat   _constant;
        GLfloat   _linear;
        GLfloat   _quadratic;

    };

    class Box
    {
    public:
        Box(QOpenGLFunctions_3_3_Core* glCore);
        ~Box();
        bool init(Shader* shader);
        void draw();
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        Shader* _shader = NULL;
        GLuint _VAO = 0;
        GLuint _VBO = 0;

        glm::vec3 _boxColor;

        const std::vector<GLfloat> _vertices;
    };

    class Box2
    {
    public:
        Box2(QOpenGLFunctions_3_3_Core* glCore);
        ~Box2();
        bool init();
        void draw();
    private:
        QOpenGLFunctions_3_3_Core* _glCore = NULL;
        GLuint _VAO = 0;
        GLuint _VBO = 0;

        glm::vec3 _boxColor;

        const std::vector<GLfloat> _vertices;
    };

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
    void resetCursor();
    void trianglePaint();
    void cubePaint();
    void lightPaint();
    void multiBoxPaint();

private:
    const char* _triangleVsFile = "shader.vs";
    const char* _triangleFsFile = "shader.fs";
    Shader*     _triangleShader = NULL;
    Triangle*   _triangle = NULL;

    const char* _triangle2VsFile = "shader2.vs";
    const char* _triangle2FsFile = "shader2.fs";
    Shader*     _triangle2Shader = NULL;
    Triangle2*  _triangle2 = NULL;

    const char*  _wallImageFile = "wall.jpg";
    TextureData* _wallTexture = NULL;

    const char* _triangle3VsFile = "shader3.vs";
    const char* _triangle3FsFile = "shader3.fs";
    Shader*     _triangle3Shader = NULL;
    Triangle3*  _triangle3 = NULL;

    const char*     _awesomefacePngFile = "awesomeface.png";
    PngTextureData* _awesomefaceTexture = NULL;

    const char* _triangle4VsFile = "shader4.vs";
    const char* _triangle4FsFile = "shader4.fs";
    Shader*     _triangle4Shader = NULL;
    Triangle4*  _triangle4 = NULL;
    glm::mat4*  _triangle4Trans = NULL;

    const char* _triangle5VsFile = "shader5.vs";
    const char* _triangle5FsFile = "shader5.fs";
    Shader*     _triangle5Shader = NULL;
    Triangle5*  _triangle5 = NULL;
    glm::mat4*  _triangle5Model = NULL;
    glm::mat4*  _triangle5View = NULL;
    glm::mat4*  _triangle5Projection = NULL;

    const char* _cubeVsFile = "cube.vs.glsl";
    const char* _cubeFsFile = "cube.fs.glsl";
    Shader*     _cubeShader = NULL;
    Cube*       _cube = NULL;
    glm::mat4*  _cubeModel = NULL;
    glm::mat4*  _cubeView = NULL;
    glm::mat4*  _cubeProjection = NULL;
    GLint       _angle = 0;

    Camera*     _camera = NULL;

    const char* _lightVsFile = "light.vs.glsl";
    const char* _lightFsFile = "light.fs.glsl";
    Shader*     _lightShader = NULL;
    Light*      _light = NULL;
    glm::mat4*  _lightModel = NULL;
    glm::mat4*  _lightProjection = NULL;

    const char* _boxVsFile = "box.vs.glsl";
    const char* _boxFsFile = "box.fs.glsl";
    Shader*     _boxShader = NULL;
    Box*        _box = NULL;
    glm::mat4*  _boxModel = NULL;

    const char*     _containerPngFile = "container.png";
    PngTextureData* _containerTexture = NULL;

    const char*     _containerSpecularPngFile = "container_specular.png";
    PngTextureData* _containerSpecularTexture = NULL;

    const char* _box2VsFile = "box2.vs.glsl";
    const char* _box2FsFile = "box2.fs.glsl";
    Shader*     _box2Shader = NULL;
    Box2*       _box2 = NULL;
    glm::mat4*  _box2Model = NULL;

    DirectLight* _directLight = NULL;
    const char*  _directLightVsFile = "DirectLight.vs.glsl";
    const char*  _directLightFsFile = "DirectLight.fs.glsl";
    Shader*      _directLightShader = NULL;
    Box2*        _directLightBox = NULL;
    glm::mat4*   _directLightBoxModel = NULL;

    PointLight*      _pointLight = NULL;
    const char*      _pointLightShapeVsFile = "PointLightShape.vs.glsl";
    const char*      _pointLightShapeFsFile = "PointLightShape.fs.glsl";
    Shader*          _pointLightShapeShader = NULL;
    PointLightShape* _pointLightShape = NULL;

    const char*      _pointLightVsFile = "PointLight.vs.glsl";
    const char*      _pointLightFsFile = "PointLight.fs.glsl";
    Shader*          _pointLightShader = NULL;
    Box2*            _pointLightBox = NULL;

    SpotLight*  _spotLight = NULL;
    const char* _spotLightVsFile = "SpotLight.vs.glsl";
    const char* _spotLightFsFile = "SpotLight.fs.glsl";
    Shader*     _spotLightShader = NULL;
    Box2*       _spotLightBox = NULL;

    const char* _multiLightVsFile = "MultiLight.vs";
    const char* _multiLightFsFile = "MultiLight.fs";
    Shader*     _multiLightShader = NULL;
    Box2*       _multiLightBox = NULL;
};
#endif // MAINWINDOW_H
