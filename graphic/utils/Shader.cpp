#include "Shader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "utils/utDefer.h"
#include "utils/OpenGLSingleton.h"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader() {}

Shader::~Shader() {}

bool Shader::init(const GLchar* vertexPath, const GLchar* fragmentPath) {
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // 保证 ifstream 对象可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // 打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        
        // 读取文件的缓冲内容到数据流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // 关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();

        // 转换数据流到 string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    } catch (std::ifstream::failure* e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    // 2. 编译着色器
    GLuint vertex, fragment;
    int    success;
    char   infoLog[512];

    // 顶点着色器
    vertex = OpenGLInstance->glCreateShader(GL_VERTEX_SHADER);
    utDefer(OpenGLInstance->glDeleteShader(vertex));

    OpenGLInstance->glShaderSource(vertex, 1, &vShaderCode, NULL);
    OpenGLInstance->glCompileShader(vertex);
    // 打印编译错误
    OpenGLInstance->glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        OpenGLInstance->glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }

    // 片段着色器
    fragment = OpenGLInstance->glCreateShader(GL_FRAGMENT_SHADER);
    utDefer(OpenGLInstance->glDeleteShader(fragment));

    OpenGLInstance->glShaderSource(fragment, 1, & fShaderCode, NULL);
    OpenGLInstance->glCompileShader(fragment);
    // 打印编译错误
    OpenGLInstance->glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        OpenGLInstance->glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }

    // 着色器程序
    _id = OpenGLInstance->glCreateProgram();
    OpenGLInstance->glAttachShader(_id, vertex);
    OpenGLInstance->glAttachShader(_id, fragment);
    OpenGLInstance->glLinkProgram(_id);
    // 打印链接错误
    OpenGLInstance->glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        OpenGLInstance->glGetProgramInfoLog(_id, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }

    return true;
}

void Shader::use() {
    OpenGLInstance->glUseProgram(_id);
}

GLuint Shader::ID() const { 
    return _id; 
}

void Shader::setBool(const std::string& name, bool value) const {
    OpenGLInstance->glUniform1i(OpenGLInstance->glGetUniformLocation(_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    OpenGLInstance->glUniform1i(OpenGLInstance->glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    OpenGLInstance->glUniform1f(OpenGLInstance->glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    OpenGLInstance->glUniformMatrix4fv(OpenGLInstance->glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    OpenGLInstance->glUniform3f(OpenGLInstance->glGetUniformLocation(_id, name.c_str()), x, y, z);
}

