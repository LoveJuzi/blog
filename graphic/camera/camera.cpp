#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : _view(glm::mat4(1.0f))
{
}

const glm::mat4& Camera::getView() const
{
    return _view;
}

void Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up)
{
    _view = glm::lookAt(eye, center, up);
}
