#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Shader.cpp
class Shader {
public:
    bool init(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use();
    unsigned int ID() const;

    //uniform 工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;

private:
    unsigned int _id;
};

#endif // SHADER_H_
