#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

Shader::Shader(QOpenGLFunctions_3_3_Core* glcore)
    : _glcore(glcore)
{

}

int Shader::build(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
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
        // 转换数据流到string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return 1;
    }
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    GLuint vertexShader = _glcore->glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = _glcore->glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = _glcore->glCreateProgram();

    _glcore->glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    _glcore->glCompileShader(vertexShader);

    _glcore->glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    _glcore->glCompileShader(fragmentShader);

    _glcore->glAttachShader(shaderProgram, vertexShader);
    _glcore->glAttachShader(shaderProgram, fragmentShader);
    _glcore->glLinkProgram(shaderProgram);

    _glcore->glDeleteShader(vertexShader);
    _glcore->glDeleteShader(fragmentShader);

    return 0;
}

void Shader::use()
{
    _glcore->glUseProgram(shaderProgram);
}

void Shader::setInt(const std::string& name, int value)
{
    _glcore->glUniform1i(_glcore->glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setMatrix4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    _glcore->glUniformMatrix4fv(_glcore->glGetUniformLocation(shaderProgram, name.c_str()),
                                1, GL_FALSE, value);
}
