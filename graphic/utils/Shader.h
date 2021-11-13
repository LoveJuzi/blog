#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_3_3_Core>

#include "glm/glm.hpp"

// Shader.cpp
class Shader {
public:
    Shader();
    ~Shader();

    bool init(const GLchar* vertexPath, const GLchar* fragmentPath);

    void use();
    GLuint ID() const;

    //uniform 工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;

private:
    GLuint _id;
};

#endif // SHADER_H
