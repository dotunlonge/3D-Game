#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace RenderEngine {
    class Model;
}

namespace RenderEngine {

/**
 * @brief Represents a game object in the 3D world
 * 
 * Contains position, rotation, scale, and a reference to a model.
 * Used for collectibles, obstacles, and other interactive objects.
 */
class GameObject {
public:
    GameObject(std::shared_ptr<Model> model, 
              const glm::vec3& position = glm::vec3(0.0f),
              const glm::vec3& scale = glm::vec3(1.0f));

    void update(float deltaTime);
    void render(const glm::mat4& view, const glm::mat4& projection);

    glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3& pos) { m_position = pos; }
    
    glm::vec3 getScale() const { return m_scale; }
    void setScale(const glm::vec3& scale) { m_scale = scale; }

    float getRotation() const { return m_rotation; }
    void setRotation(float rotation) { m_rotation = rotation; }

    bool isCollected() const { return m_collected; }
    void setCollected(bool collected) { m_collected = collected; }

    float getBoundingRadius() const { return m_boundingRadius; }

private:
    std::shared_ptr<Model> m_model;
    glm::vec3 m_position;
    glm::vec3 m_scale;
    float m_rotation;
    float m_rotationSpeed;
    bool m_collected;
    float m_boundingRadius;
    float m_bobOffset;
    float m_bobSpeed;
};

} // namespace RenderEngine

