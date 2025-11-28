#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <glad/glad.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

namespace RenderEngine {

/**
 * @brief OpenGL shader program wrapper with uniform management
 * 
 * Handles shader compilation, linking, and provides convenient
 * methods for setting uniforms. Includes error checking and
 * informative error messages.
 */
class Shader {
public:
    Shader();
    ~Shader();

    // Non-copyable, movable
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);

    void use() const;
    unsigned int getId() const { return m_id; }

    // Uniform setters
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat2(const std::string& name, const glm::mat2& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    unsigned int m_id;
    
    bool compileShader(unsigned int& shader, const std::string& source, GLenum type);
    bool linkProgram(unsigned int program);
    std::string readFile(const std::string& filepath);
    int getUniformLocation(const std::string& name) const;
};

} // namespace RenderEngine

