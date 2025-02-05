#ifndef CAMERA_H
#define CAMERA_H
//#define EULERCAM
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
enum Camera_Movement {
    UP, DOWN,
    FORWARD, BACKWARD,
    LEFT, RIGHT
};
// Default camera values
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 0.785f;
const float MAX_PITCH = 1.55334f;
const float MAX_FOV = 1.5708f;
const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
class Camera {
public:
    glm::vec3 position;
    glm::vec3 position_delta;
    glm::vec3 cameraUp;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 direction;
    glm::quat orientation;
    float fov;
    // Constructor
    Camera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f), glm::quat Orientation = glm::quat(glm::vec3(0.0f, 4.71239f, 0.0f))) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), fov(FOV), m_maxPitch(MAX_PITCH), m_maxFov(MAX_FOV)
    {
        position_delta = glm::vec3(0, 0, 0);
        position = Position;
        cameraUp = CameraUp;
        orientation = Orientation;
        updateCameraVectors();
    }
    glm::mat4 GetViewMatrix(){
        return lookAt(position, position + front, cameraUp);
    }
    void ProcessKeyboard(Camera_Movement moveDirection, float deltaTime) {
        float speed = m_movementSpeed * deltaTime;
        switch (moveDirection) {
            case FORWARD:
                position += front * speed;
                break;
            case BACKWARD:
                position -= front * speed;
                break;
            case LEFT:
                position -= right * speed;
                break;
            case RIGHT:
                position += right * speed;
                break;
#ifndef EULERCAM
            case UP:
                position += cameraUp * speed;
                break;
            case DOWN:
                position -= cameraUp * speed;
                break;
#endif
        }
    }
    void ProcessMouseMovement(float xoffset, float yoffset) {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;
        m_yaw += glm::radians(xoffset);
        m_pitch += glm::radians(yoffset);
        if (m_pitch > m_maxPitch)
            m_pitch = m_maxPitch;
        if (m_pitch < -m_maxPitch)
            m_pitch = -m_maxPitch;

        updateCameraVectors();
    }
    void ProcessMouseScroll(float yoffset) {
        fov -= glm::radians((float)yoffset);
        if (fov < 0.0174533f)
            fov = 0.0174533f;
        if (fov > m_maxFov)
            fov = m_maxFov;
    }
private:
    void updateCameraVectors() {
#ifdef EULERCAM
        front = normalize(vec3(cos(m_yaw) * cos(m_pitch),
            sin(m_pitch),
            sin(m_yaw) * cos(m_pitch)
        ));
#else
        glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::quat pitch_quat = angleAxis(m_pitch, x_axis);
        glm::quat yaw_quat = angleAxis(-m_yaw, worldUp);

        front = normalize((yaw_quat * pitch_quat) * z_axis);
#endif 
        // also re-calculate the Right and Up vector
        right = normalize(cross(front, worldUp));
        cameraUp = normalize(cross(right, front));
    }
    float m_pitch;
    float m_yaw;
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_maxPitch;
    float m_maxFov;
};
#endif