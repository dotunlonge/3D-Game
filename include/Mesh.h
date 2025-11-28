#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <glad/glad.h>
#endif
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <cstddef>

namespace RenderEngine {

/**
 * @brief Vertex structure for 3D meshes
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex)
        : position(pos), normal(norm), texCoords(tex) {}
};

/**
 * @brief 3D mesh with vertex data and OpenGL buffers
 * 
 * Manages vertex buffer objects (VBO), vertex array objects (VAO),
 * and element buffer objects (EBO) for efficient rendering.
 */
class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    // Non-copyable, movable
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void draw() const;
    unsigned int getVAO() const { return m_VAO; }
    size_t getIndexCount() const { return m_indices.size(); }

private:
    void setupMesh();

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    unsigned int m_VAO, m_VBO, m_EBO;
};

} // namespace RenderEngine

