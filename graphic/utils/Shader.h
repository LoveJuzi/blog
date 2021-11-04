#ifndef SHADER_H_
#define SHADER_H_

#if 0
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Shader.cpp
class Shader {
public:
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    void use();
    unsigned int ID() const;

    //uniform 工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    unsigned int _id;
};
#endif

#endif // SHADER_H_
