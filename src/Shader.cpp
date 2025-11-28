#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace RenderEngine {

Shader::Shader() : m_id(0) {
}

Shader::~Shader() {
    if (m_id != 0) {
        glDeleteProgram(m_id);
    }
}

Shader::Shader(Shader&& other) noexcept : m_id(other.m_id) {
    other.m_id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (m_id != 0) {
            glDeleteProgram(m_id);
        }
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);
    
    if (vertexCode.empty() || fragmentCode.empty()) {
        return false;
    }
    
    return loadFromSource(vertexCode, fragmentCode);
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int vertex, fragment;
    
    // Compile vertex shader
    if (!compileShader(vertex, vertexSource, GL_VERTEX_SHADER)) {
        return false;
    }
    
    // Compile fragment shader
    if (!compileShader(fragment, fragmentSource, GL_FRAGMENT_SHADER)) {
        glDeleteShader(vertex);
        return false;
    }
    
    // Create shader program
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    
    if (!linkProgram(m_id)) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(m_id);
        m_id = 0;
        return false;
    }
    
    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return true;
}

void Shader::use() const {
    glUseProgram(m_id);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

bool Shader::compileShader(unsigned int& shader, const std::string& source, GLenum type) {
    shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "Shader compilation error (" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "):\n" 
                  << infoLog << std::endl;
        glDeleteShader(shader);
        return false;
    }
    
    return true;
}

bool Shader::linkProgram(unsigned int program) {
    glLinkProgram(program);
    
    int success;
    char infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "Shader linking error:\n" << infoLog << std::endl;
        return false;
    }
    
    return true;
}

std::string Shader::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int Shader::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(m_id, name.c_str());
}

} // namespace RenderEngine

