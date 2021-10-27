#include "globject.h"

GLObject::GLObject(QOpenGLFunctions_3_3_Core *glcore)
    : _glcore(glcore), _shader(new Shader(glcore)),
      _model(glm::mat4(1.0f)), _view(glm::mat4(1.0f)), _projection(glm::mat4(1.0f))
{
}

GLObject::~GLObject()
{

}

void GLObject::setView(const glm::mat4 &mat)
{
    _view = mat;
}

void GLObject::setModel(const glm::mat4 &mat)
{
    _model = mat;
}

void GLObject::setProjection(const glm::mat4 &mat)
{
    _projection = mat;
}
