#pragma once

#include <memory>
#include <vector>
#include <random>
#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Shader.h"

namespace RenderEngine {

/**
 * @brief Main game class managing game state, objects, and gameplay
 * 
 * Handles:
 * - Game loop (update/render)
 * - Collision detection
 * - Score tracking
 * - Game object management
 * - Input handling
 */
class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void shutdown();

private:
    void update(float deltaTime);
    void render();
    void processInput(float deltaTime);
    void checkCollisions();
    void spawnCollectible();
    void updateUI();

    std::unique_ptr<Window> m_window;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Renderer> m_renderer;

    std::vector<std::shared_ptr<GameObject>> m_collectibles;
    std::shared_ptr<Model> m_groundModel;
    std::shared_ptr<Model> m_collectibleModel;
    std::shared_ptr<Shader> m_groundShader;
    std::shared_ptr<Shader> m_collectibleShader;

    // Game state
    int m_score;
    int m_collectiblesCollected;
    float m_gameTime;
    bool m_running;

    // World bounds
    float m_worldSize;
    int m_maxCollectibles;

    // Input state
    bool m_firstMouse;
    double m_lastMouseX;
    double m_lastMouseY;

    // Random number generation
    std::mt19937 m_rng;
    std::uniform_real_distribution<float> m_posDistribution;
    std::uniform_real_distribution<float> m_rotationDistribution;
};

} // namespace RenderEngine

