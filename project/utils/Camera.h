#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   0.5f;
const float SENSITIVITY =   0.001f;
//const float SENSITIVITY =   2.5f;
const float ZOOM        =  45.0f;

// Camera.cpp 
class Camera {
public:
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW, 
            float pitch = PITCH);

    glm::mat4 getViewMatrix();
    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

    float getZoom() const { return _zoom; }

private:
    void updateCameraVectors();

private:
    // camera Attributes
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    // euler Angles
    float _yaw;
    float _pitch;

    // camera options
    float _movementSpeed;
    float _mouseSensitivity;
    float _zoom;
};

#endif // CAMERA_H_
