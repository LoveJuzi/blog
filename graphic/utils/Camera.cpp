#include "Camera.h"

#include <iostream>

#include "glm/gtx/string_cast.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : _front(glm::vec3(0.0f, 0.0f, -1.0f)), _movementSpeed(SPEED),
    _mouseSensitivity(SENSITIVITY), _zoom(ZOOM)
{
    _position = position;
    _worldUp = up;
    _yaw = yaw;
    _pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

glm::vec3 Camera::getPosition() const {
    return _position;
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    // also re-calculate the Right and Up vector
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up    = glm::normalize(glm::cross(_right, _front));
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = _movementSpeed * deltaTime;
    if (direction == FORWARD) {
        _position += _front * velocity;
    }
    if (direction == BACKWARD) {
        _position -= _front * velocity;
    }
    if (direction == LEFT) {
        _position -= _right * velocity;
    }
    if (direction == RIGHT) {
        _position += _right * velocity;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constraintPitch) {
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    _yaw += xoffset;
    _pitch += yoffset;

    if (constraintPitch && _pitch >  89.0f) _pitch = 89.0f;
    if (constraintPitch && _pitch < -89.0f) _pitch = -89.0f;

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    _zoom -= (float)yoffset;
    if (_zoom < 1.0f) {
        _zoom = 1.0f;
    } else if (_zoom > 45.0f) {
        _zoom = 45.0f;
    }
}
