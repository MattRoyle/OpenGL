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
using namespace glm;
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
const vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
class Camera {
public:
    vec3 position;
    vec3 position_delta;
    vec3 cameraUp;
    vec3 front;
    vec3 right;
    vec3 direction;
    quat orientation;
    float fov;
    // Constructor
    Camera(vec3 Position = vec3(0.0f, 0.0f, 0.0f), vec3 CameraUp = vec3(0.0f, 1.0f, 0.0f), quat Orientation = quat(vec3(0.0f, 4.71239f, 0.0f))) :
        front(vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), fov(FOV), m_maxPitch(MAX_PITCH), m_maxFov(MAX_FOV)
    {
        position_delta = glm::vec3(0, 0, 0);
        position = Position;
        cameraUp = CameraUp;
        orientation = Orientation;
        updateCameraVectors();
    }
    mat4 GetViewMatrix(){
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
        m_yaw += radians(xoffset);
        m_pitch += radians(yoffset);
        if (m_pitch > m_maxPitch)
            m_pitch = m_maxPitch;
        if (m_pitch < -m_maxPitch)
            m_pitch = -m_maxPitch;

        updateCameraVectors();
    }
    void ProcessMouseScroll(float yoffset) {
        fov -= radians((float)yoffset);
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
        vec3 x_axis = vec3(1.0f, 0.0f, 0.0f);
        vec3 z_axis = vec3(0.0f, 0.0f, -1.0f);

        quat pitch_quat = angleAxis(m_pitch, x_axis);
        quat yaw_quat = angleAxis(-m_yaw, worldUp);

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