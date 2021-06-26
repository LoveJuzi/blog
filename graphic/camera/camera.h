#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    const glm::mat4& getView() const;

    void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

private:
    glm::mat4 _view;
};

#endif // CAMERA_H
