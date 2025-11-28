#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace RenderEngine {

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr) {
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    // Load OpenGL functions using GLAD
    #ifdef GLAD_GL_IMPLEMENTATION
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }
    #elif defined(__APPLE__)
    // macOS: OpenGL functions are available via framework
    // No explicit loading needed
    #else
    // For other systems, GLAD should be used
    // Generate GLAD from https://glad.dav1d.de/ and include glad.c
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD. Please generate GLAD from https://glad.dav1d.de/" << std::endl;
        return;
    }
    #endif

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::isKeyPressed(int key) const {
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Window::isMouseButtonPressed(int button) const {
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

void Window::getMousePosition(double& x, double& y) const {
    glfwGetCursorPos(m_window, &x, &y);
}

void Window::setMousePosition(double x, double y) {
    glfwSetCursorPos(m_window, x, y);
}

void Window::setCursorMode(int mode) {
    glfwSetInputMode(m_window, GLFW_CURSOR, mode);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_width = width;
        win->m_height = height;
    }
}

} // namespace RenderEngine

