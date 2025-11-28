cmake_minimum_required(VERSION 3.15)
project(RenderEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# macOS-specific: Use system OpenGL framework
if(APPLE)
    find_library(OPENGL_FRAMEWORK OpenGL)
    if(OPENGL_FRAMEWORK)
        message(STATUS "Using macOS OpenGL framework")
    endif()
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "include/*.h"
)

# Add GLAD source if it exists (optional on macOS)
if(EXISTS "${CMAKE_SOURCE_DIR}/src/glad.c")
    list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/src/glad.c")
    add_definitions(-DGLAD_GL_IMPLEMENTATION)
    message(STATUS "Using GLAD loader from src/glad.c")
elseif(APPLE)
    # On macOS, we can use the system OpenGL framework
    message(STATUS "GLAD source not found - using macOS OpenGL framework")
    message(STATUS "For best cross-platform support, generate GLAD from https://glad.dav1d.de/")
else()
    message(WARNING "GLAD source (src/glad.c) not found. Please generate from https://glad.dav1d.de/")
endif()

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
if(APPLE)
    target_link_libraries(${PROJECT_NAME} 
        ${OPENGL_FRAMEWORK}
        glfw
        glm::glm
    )
else()
    target_link_libraries(${PROJECT_NAME} 
        OpenGL::GL
        glfw
        glm::glm
    )
endif()

# Shaders are embedded in the code, no need to copy
# Uncomment if you add external shader files:
# file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Compiler-specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

