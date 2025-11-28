#include "Game.h"
#include <iostream>
#include <cstdlib>

/**
 * @file main.cpp
 * @brief Entry point for the Render Engine 3D game
 * 
 * This is a demonstration of a modern C++ game engine featuring:
 * - Clean architecture with separation of concerns
 * - Modern C++17 features (smart pointers, RAII, move semantics)
 * - OpenGL 3.3+ rendering pipeline
 * - First-person camera controls
 * - Collision detection
 * - Procedural object spawning
 * - Beautiful lighting and shader effects
 */

int main() {
    using namespace RenderEngine;

    try {
        Game game;
        
        if (!game.initialize()) {
            std::cerr << "Failed to initialize game" << std::endl;
            return EXIT_FAILURE;
        }

        game.run();
        game.shutdown();

        std::cout << "\nThanks for playing!" << std::endl;
        return EXIT_SUCCESS;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

