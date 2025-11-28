#include "Game.h"
#include "Model.h"
#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {

Game::Game()
    : m_score(0)
    , m_collectiblesCollected(0)
    , m_gameTime(0.0f)
    , m_running(false)
    , m_worldSize(20.0f)
    , m_maxCollectibles(15)
    , m_firstMouse(true)
    , m_lastMouseX(0.0)
    , m_lastMouseY(0.0)
    , m_rng(std::random_device{}())
    , m_posDistribution(-m_worldSize * 0.4f, m_worldSize * 0.4f)
    , m_rotationDistribution(0.0f, 360.0f) {
}

Game::~Game() {
    shutdown();
}

bool Game::initialize() {
    // Create window
    m_window = std::make_unique<Window>(1280, 720, "Render Engine - 3D Game");
    if (!m_window->getHandle()) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }

    // Create camera
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 2.0f, 5.0f));
    m_camera->setSpeed(8.0f);
    m_camera->setSensitivity(0.15f);

    // Create renderer
    m_renderer = std::make_unique<Renderer>();

    // Create models
    m_groundModel = Model::createPlane(m_worldSize);
    m_collectibleModel = Model::createSphere(16);

    // Create shaders
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

    const std::string groundFragmentSource = R"(
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
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Grid pattern
    vec2 grid = abs(fract(TexCoord * 10.0 - 0.5) - 0.5) / fwidth(TexCoord * 10.0);
    float gridLine = min(grid.x, grid.y);
    vec3 gridColor = mix(vec3(0.2, 0.2, 0.25), vec3(0.3, 0.3, 0.35), smoothstep(0.0, 1.0, gridLine));

    vec3 result = (ambient + diffuse + specular) * gridColor;
    FragColor = vec4(result, 1.0);
}
)";

    const std::string collectibleFragmentSource = R"(
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
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Glowing effect
    vec3 glowColor = vec3(0.8 + sin(time * 3.0 + FragPos.x * 10.0) * 0.2,
                          0.9 + cos(time * 2.5 + FragPos.z * 10.0) * 0.1,
                          1.0);
    
    vec3 result = (ambient + diffuse * 1.5 + specular * 2.0) * glowColor;
    FragColor = vec4(result, 1.0);
}
)";

    m_groundShader = std::make_shared<Shader>();
    if (!m_groundShader->loadFromSource(vertexSource, groundFragmentSource)) {
        std::cerr << "Failed to create ground shader" << std::endl;
        return false;
    }

    m_collectibleShader = std::make_shared<Shader>();
    if (!m_collectibleShader->loadFromSource(vertexSource, collectibleFragmentSource)) {
        std::cerr << "Failed to create collectible shader" << std::endl;
        return false;
    }

    // Spawn initial collectibles
    for (int i = 0; i < m_maxCollectibles; ++i) {
        spawnCollectible();
    }

    // Lock cursor
    m_window->setCursorMode(GLFW_CURSOR_DISABLED);

    std::cout << "\n=== Render Engine - 3D Game ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "  Collect the glowing spheres!" << std::endl;
    std::cout << "==============================\n" << std::endl;

    m_running = true;
    return true;
}

void Game::run() {
    if (!m_running) return;

    double lastTime = glfwGetTime();
    double lastFrameTime = lastTime;

    while (m_running && !m_window->shouldClose()) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        // Cap delta time to prevent large jumps
        deltaTime = std::min(deltaTime, 0.1f);

        processInput(deltaTime);
        update(deltaTime);
        render();

        m_window->swapBuffers();
        m_window->pollEvents();
    }
}

void Game::shutdown() {
    m_running = false;
}

void Game::update(float deltaTime) {
    m_gameTime += deltaTime;

    // Update collectibles
    for (auto& collectible : m_collectibles) {
        collectible->update(deltaTime);
    }

    // Check collisions
    checkCollisions();

    // Respawn collected items
    auto it = std::remove_if(m_collectibles.begin(), m_collectibles.end(),
        [this](const std::shared_ptr<GameObject>& obj) {
            if (obj->isCollected()) {
                spawnCollectible();
                return true;
            }
            return false;
        });
    m_collectibles.erase(it, m_collectibles.end());
}

void Game::render() {
    m_window->clear(0.1f, 0.1f, 0.15f, 1.0f);

    float aspectRatio = static_cast<float>(m_window->getWidth()) / static_cast<float>(m_window->getHeight());
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(aspectRatio);

    m_renderer->setViewMatrix(view);
    m_renderer->setProjectionMatrix(projection);
    m_renderer->setViewPosition(m_camera->getPosition());

    // Render ground
    glm::mat4 groundModel = glm::mat4(1.0f);
    groundModel = glm::translate(groundModel, glm::vec3(0.0f, -0.5f, 0.0f));
    
    m_groundShader->use();
    m_groundShader->setMat4("model", groundModel);
    m_groundShader->setMat4("view", view);
    m_groundShader->setMat4("projection", projection);
    m_groundShader->setVec3("viewPos", m_camera->getPosition());
    m_groundShader->setVec3("lightPos", glm::vec3(5.0f, 10.0f, 5.0f));
    m_groundShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_groundShader->setFloat("shininess", 32.0f);
    m_groundShader->setFloat("time", m_gameTime);
    m_groundModel->draw();

    // Render collectibles
    for (const auto& collectible : m_collectibles) {
        if (collectible->isCollected()) continue;

        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 pos = collectible->getPosition();
        float bob = std::sin(m_gameTime * 2.0f + pos.x * 5.0f + pos.z * 5.0f) * 0.1f;
        model = glm::translate(model, pos + glm::vec3(0.0f, bob, 0.0f));
        model = glm::rotate(model, glm::radians(collectible->getRotation()), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, collectible->getScale());

        m_collectibleShader->use();
        m_collectibleShader->setMat4("model", model);
        m_collectibleShader->setMat4("view", view);
        m_collectibleShader->setMat4("projection", projection);
        m_collectibleShader->setVec3("viewPos", m_camera->getPosition());
        m_collectibleShader->setVec3("lightPos", glm::vec3(5.0f, 10.0f, 5.0f));
        m_collectibleShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        m_collectibleShader->setFloat("shininess", 64.0f);
        m_collectibleShader->setFloat("time", m_gameTime);
        m_collectibleModel->draw();
    }

    updateUI();
}

void Game::processInput(float deltaTime) {
    // Exit on ESC
    if (m_window->isKeyPressed(GLFW_KEY_ESCAPE)) {
        m_running = false;
    }

    // Movement
    int direction = 0;
    if (m_window->isKeyPressed(GLFW_KEY_W)) direction |= 0x01;
    if (m_window->isKeyPressed(GLFW_KEY_S)) direction |= 0x02;
    if (m_window->isKeyPressed(GLFW_KEY_A)) direction |= 0x04;
    if (m_window->isKeyPressed(GLFW_KEY_D)) direction |= 0x08;

    m_camera->processKeyboard(direction, deltaTime);

    // Mouse look
    double mouseX, mouseY;
    m_window->getMousePosition(mouseX, mouseY);

    if (m_firstMouse) {
        m_lastMouseX = mouseX;
        m_lastMouseY = mouseY;
        m_firstMouse = false;
    }

    float xoffset = static_cast<float>(mouseX - m_lastMouseX);
    float yoffset = static_cast<float>(m_lastMouseY - mouseY);

    m_lastMouseX = mouseX;
    m_lastMouseY = mouseY;

    m_camera->processMouseMovement(xoffset, yoffset);
}

void Game::checkCollisions() {
    glm::vec3 cameraPos = m_camera->getPosition();
    float collisionRadius = 0.8f;

    for (auto& collectible : m_collectibles) {
        if (collectible->isCollected()) continue;

        glm::vec3 objPos = collectible->getPosition();
        float distance = glm::length(cameraPos - objPos);
        float minDistance = collisionRadius + collectible->getBoundingRadius();

        if (distance < minDistance) {
            collectible->setCollected(true);
            m_score += 10;
            m_collectiblesCollected++;
            std::cout << "Collected! Score: " << m_score << " (Total: " << m_collectiblesCollected << ")" << std::endl;
        }
    }
}

void Game::spawnCollectible() {
    glm::vec3 position(
        m_posDistribution(m_rng),
        0.5f + m_posDistribution(m_rng) * 0.1f,
        m_posDistribution(m_rng)
    );

    float scale = 0.3f + (m_rng() % 100) / 1000.0f;
    auto collectible = std::make_shared<GameObject>(
        m_collectibleModel,
        position,
        glm::vec3(scale)
    );

    collectible->setRotation(m_rotationDistribution(m_rng));
    m_collectibles.push_back(collectible);
}

void Game::updateUI() {
    // In a real implementation, you'd render text here
    // For now, we output to console
    static double lastPrintTime = 0.0;
    double currentTime = glfwGetTime();
    if (currentTime - lastPrintTime > 5.0) {
        std::cout << "Score: " << m_score << " | Collected: " << m_collectiblesCollected 
                  << " | Time: " << static_cast<int>(m_gameTime) << "s" << std::endl;
        lastPrintTime = currentTime;
    }
}

} // namespace RenderEngine

