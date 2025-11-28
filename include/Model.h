#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <glad/glad.h>
#endif
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

namespace RenderEngine {
    class Mesh;
    class Shader;
}

namespace RenderEngine {

/**
 * @brief 3D model container holding multiple meshes
 * 
 * Can load models from files (using Assimp) or be constructed
 * programmatically. For this demo, we'll create simple geometric
 * models programmatically.
 */
class Model {
public:
    Model();
    ~Model() = default;

    void addMesh(std::shared_ptr<Mesh> mesh);
    void draw(std::shared_ptr<Shader> shader) const;
    void draw() const;

    size_t getMeshCount() const { return m_meshes.size(); }

    // Factory methods for creating simple shapes
    static std::shared_ptr<Model> createCube();
    static std::shared_ptr<Model> createSphere(int segments = 32);
    static std::shared_ptr<Model> createPlane(float size = 1.0f);

private:
    std::vector<std::shared_ptr<Mesh>> m_meshes;
};

} // namespace RenderEngine

