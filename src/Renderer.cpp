#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include <iostream>

namespace RenderEngine {

Renderer::Renderer() {
    // Create default shader with embedded source
    m_defaultShader = std::make_shared<Shader>();
    
    const std::string vertexSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

    const std::string fragmentSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float shininess;
uniform float time;

void main() {
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    // Add some color variation based on position for collectibles
    if (objectColor.r > 0.8 && objectColor.g > 0.8) {
        result += vec3(sin(time * 2.0 + FragPos.x * 5.0) * 0.2,
                      cos(time * 2.0 + FragPos.z * 5.0) * 0.2, 0.0);
    }
    
    FragColor = vec4(result, 1.0);
}
)";

    if (!m_defaultShader->loadFromSource(vertexSource, fragmentSource)) {
        std::cerr << "Failed to create default shader" << std::endl;
    }

    enableDepthTest(true);
    setClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

Renderer::~Renderer() {
}

void Renderer::beginFrame() {
    // Frame setup is handled by Window::clear()
}

void Renderer::endFrame() {
    // Frame cleanup if needed
}

void Renderer::setViewMatrix(const glm::mat4& view) {
    m_viewMatrix = view;
}

void Renderer::setProjectionMatrix(const glm::mat4& projection) {
    m_projectionMatrix = projection;
}

void Renderer::setViewPosition(const glm::vec3& position) {
    m_viewPosition = position;
}

void Renderer::drawMesh(const Mesh& mesh, const glm::mat4& model) {
    m_defaultShader->use();
    m_defaultShader->setMat4("model", model);
    m_defaultShader->setMat4("view", m_viewMatrix);
    m_defaultShader->setMat4("projection", m_projectionMatrix);
    m_defaultShader->setVec3("viewPos", m_viewPosition);
    
    // Default lighting
    m_defaultShader->setVec3("lightPos", glm::vec3(5.0f, 10.0f, 5.0f));
    m_defaultShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_defaultShader->setFloat("shininess", 32.0f);
    m_defaultShader->setVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));
    
    mesh.draw();
}

void Renderer::drawModel(const Model& model, const glm::mat4& modelMatrix) {
    m_defaultShader->use();
    m_defaultShader->setMat4("model", modelMatrix);
    m_defaultShader->setMat4("view", m_viewMatrix);
    m_defaultShader->setMat4("projection", m_projectionMatrix);
    m_defaultShader->setVec3("viewPos", m_viewPosition);
    
    // Default lighting
    m_defaultShader->setVec3("lightPos", glm::vec3(5.0f, 10.0f, 5.0f));
    m_defaultShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_defaultShader->setFloat("shininess", 32.0f);
    m_defaultShader->setVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));
    
    model.draw();
}

void Renderer::enableDepthTest(bool enable) {
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::enableBlending(bool enable) {
    if (enable) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
}

void Renderer::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

} // namespace RenderEngine

