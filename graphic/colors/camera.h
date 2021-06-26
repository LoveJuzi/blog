#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_3_2_Core>
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    const glm::mat4& getView();

    void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDow();
    void rotate(GLint xoffset, GLint yoffset);

private:
    glm::mat4 _view;

    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    GLfloat   _cameraSpeed;
    GLint     _dealtaTime;
    GLint     _lastTime;
    GLfloat   _pitch;
    GLfloat   _yaw;

    bool      _updateView;
};

#endif // CAMERA_H
