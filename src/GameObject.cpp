#include "GameObject.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace RenderEngine {

GameObject::GameObject(std::shared_ptr<Model> model, const glm::vec3& position, const glm::vec3& scale)
    : m_model(model)
    , m_position(position)
    , m_scale(scale)
    , m_rotation(0.0f)
    , m_rotationSpeed(45.0f)
    , m_collected(false)
    , m_boundingRadius(0.5f * glm::max(glm::max(scale.x, scale.y), scale.z))
    , m_bobOffset(0.0f)
    , m_bobSpeed(2.0f) {
}

void GameObject::update(float deltaTime) {
    if (m_collected) return;

    // Rotate
    m_rotation += m_rotationSpeed * deltaTime;
    if (m_rotation >= 360.0f) {
        m_rotation -= 360.0f;
    }

    // Bob up and down
    m_bobOffset += m_bobSpeed * deltaTime;
    const float twoPi = 2.0f * 3.14159265358979323846f;
    if (m_bobOffset >= twoPi) {
        m_bobOffset -= twoPi;
    }
}

void GameObject::render(const glm::mat4& view, const glm::mat4& projection) {
    if (m_collected) return;

    // Calculate model matrix with bobbing
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::translate(model, glm::vec3(0.0f, std::sin(m_bobOffset) * 0.1f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, m_scale);

    // Note: Actual rendering is handled by the Renderer class
    // This method is kept for future use or if we want to add
    // object-specific rendering logic
}

} // namespace RenderEngine

