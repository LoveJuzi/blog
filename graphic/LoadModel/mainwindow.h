#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <vector>

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
{for_each(x.begin(),x.end(),_STL_DELETE_OPERATOR_()); \
    x.clear();}
#endif

#ifndef _DISABLE_COPY_
#define _DISABLE_COPY_(x) \
  private:\
    x(const x&);\
    x& operator =(const x&);
#endif

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


#define MAX_BONE_INFULENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    int   BoneIDs[MAX_BONE_INFULENCE];
    float Weights[MAX_BONE_INFULENCE];
};

struct Texture {
    unsigned int id;
    std::string  type;
    std::string  path;
};

struct Mesh {
public:
    // 函数
    Mesh(QOpenGLFunctions_3_3_Core* glCore,
         const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<Texture>& textures);
    ~Mesh();
    void Draw(Shader& shader);

private:
    void setupMesh();

private:
    // 网格数据
    std::vector<Vertex>       _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture>      _textures;

    // 渲染数据
    unsigned int _VAO, _VBO, _EBO;

    QOpenGLFunctions_3_3_Core* _glCore = NULL;
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


class Model
{
public:
    Model(QOpenGLFunctions_3_3_Core* glCore, const char* path);
    ~Model();
    void Draw(Shader& shader);

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                              std::string typeName);
private:
    std::vector<Mesh*> _meshes;
    std::string        _directory;

    QOpenGLFunctions_3_3_Core* _glCore = NULL;

    std::vector<Texture> _texturesLoaded;
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
    void resetCursor();

private:
    glm::mat4 _projection;
    Camera*   _camera = NULL;

    const char* _modelVsFile= "model.vs";
    const char* _modelFsFile= "model.fs";
    Shader*     _modelShader = NULL;
    Model*      _model = NULL;
    Box2*       _box = NULL;
};
#endif // MAINWINDOW_H
