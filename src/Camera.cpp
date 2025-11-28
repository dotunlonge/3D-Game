#include "Camera.h"
#include <algorithm>

namespace RenderEngine {

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position)
    , m_worldUp(up)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_movementSpeed(5.0f)
    , m_mouseSensitivity(0.1f)
    , m_zoom(45.0f)
    , m_velocity(0.0f)
    , m_acceleration(20.0f)
    , m_friction(15.0f) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(m_zoom), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboard(int direction, float deltaTime) {
    // Movement velocity is handled by m_velocity member
    glm::vec3 desiredVelocity(0.0f);

    if (direction & 0x01) // Forward
        desiredVelocity += m_front;
    if (direction & 0x02) // Backward
        desiredVelocity -= m_front;
    if (direction & 0x04) // Left
        desiredVelocity -= m_right;
    if (direction & 0x08) // Right
        desiredVelocity += m_right;

    if (glm::length(desiredVelocity) > 0.0f) {
        desiredVelocity = glm::normalize(desiredVelocity) * m_movementSpeed;
    }

    // Smooth acceleration/deceleration
    glm::vec3 acceleration = (desiredVelocity - m_velocity) * m_acceleration;
    m_velocity += acceleration * deltaTime;

    // Apply friction
    m_velocity *= (1.0f - m_friction * deltaTime);
    if (glm::length(m_velocity) < 0.01f) {
        m_velocity = glm::vec3(0.0f);
    }

    m_position += m_velocity * deltaTime;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
        m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    m_zoom -= yoffset;
    m_zoom = std::clamp(m_zoom, 1.0f, 90.0f);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

} // namespace RenderEngine

