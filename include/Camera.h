#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {

/**
 * @brief First-person camera with smooth movement and mouse look
 * 
 * Implements a standard FPS camera with:
 * - WASD movement
 * - Mouse look (pitch/yaw)
 * - Smooth acceleration/deceleration
 * - Configurable sensitivity and speed
 */
class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getFront() const { return m_front; }

    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

    void setPosition(const glm::vec3& pos) { m_position = pos; }
    void setSpeed(float speed) { m_movementSpeed = speed; }
    void setSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }

private:
    void updateCameraVectors();

    // Camera attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    // Euler angles
    float m_yaw;
    float m_pitch;

    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    // Movement state
    glm::vec3 m_velocity;
    float m_acceleration;
    float m_friction;
};

} // namespace RenderEngine

