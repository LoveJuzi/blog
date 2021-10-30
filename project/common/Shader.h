#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

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

private:
    unsigned int _id;
};

#endif // SHADER_H
