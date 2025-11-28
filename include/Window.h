#pragma once

#include <GLFW/glfw3.h>
#ifdef __APPLE__
    // macOS: Include OpenGL via framework
    #include <OpenGL/gl3.h>
#else
    #include <glad/glad.h>
#endif
#include <string>

namespace RenderEngine {

/**
 * @brief Manages the application window and OpenGL context
 * 
 * This class handles window creation, event polling, and provides
 * access to window properties. It uses GLFW for cross-platform
 * window management.
 */
class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Non-copyable
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    GLFWwindow* getHandle() const { return m_window; }

    bool isKeyPressed(int key) const;
    bool isMouseButtonPressed(int button) const;
    void getMousePosition(double& x, double& y) const;
    void setMousePosition(double x, double y);
    void setCursorMode(int mode);

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

} // namespace RenderEngine

