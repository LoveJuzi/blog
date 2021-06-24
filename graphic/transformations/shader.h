#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_3_3_Core>

class Shader
{
public:
    Shader(QOpenGLFunctions_3_3_Core* glcore);

    int build(const GLchar* vertexPath, const GLchar* fragmentPath);

    void use();

    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, int value);

    GLuint getID() const { return shaderProgram; }
private:
    QOpenGLFunctions_3_3_Core* _glcore;

    GLuint shaderProgram;
};

#endif // SHADER_H
