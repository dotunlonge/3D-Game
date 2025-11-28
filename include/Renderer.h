#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace RenderEngine {

class Shader;
class Mesh;
class Model;

/**
 * @brief High-level rendering system managing shaders, meshes, and draw calls
 * 
 * Provides a clean interface for rendering 3D objects with proper
 * state management and optimization.
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    void beginFrame();
    void endFrame();
    
    void setViewMatrix(const glm::mat4& view);
    void setProjectionMatrix(const glm::mat4& projection);
    void setViewPosition(const glm::vec3& position);

    void drawMesh(const Mesh& mesh, const glm::mat4& model = glm::mat4(1.0f));
    void drawModel(const Model& model, const glm::mat4& modelMatrix = glm::mat4(1.0f));

    void enableDepthTest(bool enable = true);
    void enableBlending(bool enable = true);
    void setClearColor(float r, float g, float b, float a);

private:
    std::shared_ptr<Shader> m_defaultShader;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::vec3 m_viewPosition;
};

} // namespace RenderEngine

