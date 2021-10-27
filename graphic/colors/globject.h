#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <memory>

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

#include "shader.h"

class GLObject
{
public:
    GLObject(QOpenGLFunctions_3_3_Core *glcore);
    virtual ~GLObject();

    virtual int init() = 0;
    virtual int paint() = 0;

    void setModel(const glm::mat4& mat);
    void setView(const glm::mat4& mat);
    void setProjection(const glm::mat4& mat);

protected:
    QOpenGLFunctions_3_3_Core *_glcore;
    std::unique_ptr<Shader> _shader;

    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;

};

#endif // GLOBJECT_H
