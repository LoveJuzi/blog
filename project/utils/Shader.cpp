#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "utDefer.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
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

    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    // 2. 编译着色器
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];


    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    utDefer(glDeleteShader(vertex));

    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // 打印编译错误
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    utDefer(glDeleteShader(fragment));

    glShaderSource(fragment, 1, & fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);
    // 打印链接错误
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void Shader::use() {
    glUseProgram(_id);
}

unsigned int Shader::ID() const {
    return _id;
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
