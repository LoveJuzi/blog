#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : _view(glm::mat4(1.0f))
{
    _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    _cameraSpeed = 0.05f;
    _dealtaTime = 0.0f;
    _lastTime = 0.0f;
    _pitch = 0.0f;
    _yaw = -90.0f;
    _updateView = true;
}

const glm::mat4& Camera::getView()
{
    if (_updateView) _view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
    _updateView = false;
    return _view;
}

void Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
    _cameraPos = eye;
    _cameraFront = glm::normalize(center - _cameraPos);
    _cameraUp = glm::normalize(up);
    _updateView = true;
}

void Camera::moveLeft()
{
    _updateView = true;
    _cameraPos = _cameraPos - _cameraSpeed * glm::normalize(glm::cross(_cameraFront, _cameraUp));
}

void Camera::moveRight()
{
    _updateView = true;
    _cameraPos = _cameraPos + _cameraSpeed * glm::normalize(glm::cross(_cameraFront, _cameraUp));
}

void Camera::moveUp()
{
    _updateView = true;
    _cameraPos = _cameraPos + _cameraSpeed * _cameraFront;
}

void Camera::moveDow()
{
    _updateView = true;
    _cameraPos = _cameraPos - _cameraSpeed * _cameraFront;
}

void Camera::rotate(GLint xoffset, GLint yoffset)
{
    _updateView = true;
    float sensitivity = 0.005f;
    _pitch += yoffset * sensitivity;
    _yaw += xoffset * sensitivity;

    if (_pitch >= 89.0f) _pitch = 89.0;
    if (_pitch <= -89.0f) _pitch = -89.0;

    _cameraFront = glm::normalize(glm::vec3(
                                      cos(_pitch)*cos(_yaw),
                                      sin(_pitch),
                                      cos(_pitch)*sin(_yaw)));
}
