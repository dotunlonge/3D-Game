# Render Engine - 3D Game Engine

A modern, feature-rich C++ game engine demonstrating professional software engineering practices. This engine features a beautiful 3D rendering pipeline, smooth first-person controls, and engaging gameplay mechanics.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3-orange.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 🎮 Features

### Core Engine
- **Modern C++17 Architecture**: Clean separation of concerns with RAII, smart pointers, and move semantics
- **OpenGL 3.3+ Rendering Pipeline**: Efficient GPU-accelerated 3D graphics
- **First-Person Camera**: Smooth WASD movement with mouse look and acceleration/deceleration
- **Shader System**: Custom GLSL shaders with Phong lighting, specular highlights, and dynamic effects
- **Mesh & Model System**: Efficient vertex buffer management with VAO/VBO/EBO
- **Collision Detection**: Sphere-based collision system for game objects

### Gameplay
- **Collectible System**: Glowing spheres that rotate and bob with dynamic lighting
- **Procedural Spawning**: Randomly placed collectibles with varied scales
- **Score Tracking**: Real-time score and statistics
- **Beautiful Visuals**: 
  - Grid-patterned ground plane
  - Glowing collectibles with pulsing effects
  - Dynamic lighting with ambient, diffuse, and specular components
  - Smooth camera movement

## 🏗️ Architecture

### Class Structure

```
RenderEngine/
├── Window          - GLFW window management and input handling
├── Camera          - First-person camera with smooth controls
├── Renderer        - High-level rendering system
├── Shader          - OpenGL shader program wrapper
├── Mesh            - Vertex buffer and rendering data
├── Model           - Container for multiple meshes
├── GameObject      - Game entity with position, rotation, scale
└── Game            - Main game loop and state management
```

### Design Patterns
- **RAII**: Automatic resource management (OpenGL buffers, shaders)
- **Smart Pointers**: `std::shared_ptr` and `std::unique_ptr` for memory safety
- **Move Semantics**: Efficient resource transfer
- **Factory Methods**: `Model::createCube()`, `Model::createSphere()`, etc.
- **Component-Based**: Separated rendering, physics, and game logic

## 📦 Dependencies

- **CMake** 3.15+ (build system)
- **OpenGL** 3.3+ (graphics API)
- **GLFW** 3.x (window management)
- **GLM** (mathematics library)
- **GLAD** (OpenGL function loader)

### Installing Dependencies

#### macOS
```bash
brew install glfw glm
# GLAD needs to be generated from https://glad.dav1d.de/
# Select OpenGL 3.3 Core profile
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libglfw3-dev libglm-dev
# Generate GLAD from https://glad.dav1d.de/
```

#### Windows
Use vcpkg or download pre-built binaries:
```bash
vcpkg install glfw3 glm
```

## 🔨 Building

### Prerequisites
1. Generate GLAD headers:
   - Visit https://glad.dav1d.de/
   - Select: **OpenGL 3.3**, **Core** profile
   - Generate and extract to `include/glad/`

### Build Steps

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run
./RenderEngine
```

### CMake Options
- `CMAKE_BUILD_TYPE`: `Release` or `Debug` (default: `Release`)

## 🎯 Controls

- **W/A/S/D** - Move forward/left/backward/right
- **Mouse** - Look around (first-person camera)
- **ESC** - Exit game

## 🎨 Technical Highlights

### Rendering Pipeline
1. **Vertex Shader**: Transforms vertices to clip space, calculates world positions
2. **Fragment Shader**: Implements Phong lighting model with:
   - Ambient lighting (base illumination)
   - Diffuse lighting (Lambertian reflection)
   - Specular highlights (Blinn-Phong)
   - Dynamic color effects for collectibles

### Performance Optimizations
- **Indexed Rendering**: Uses EBO for efficient vertex reuse
- **Static Buffers**: Mesh data uploaded once to GPU
- **Batch Rendering**: Multiple objects share shader programs
- **Depth Testing**: Early Z-culling for hidden surface removal

### Code Quality
- **Comprehensive Documentation**: Doxygen-style comments
- **Error Handling**: Proper error checking and reporting
- **Modern C++**: Uses latest language features appropriately
- **Clean Code**: Readable, maintainable, and well-structured

## 📊 Project Statistics

- **Lines of Code**: ~2000+ (excluding comments)
- **Classes**: 8 core engine classes
- **Shaders**: 3+ custom GLSL shaders
- **Dependencies**: Minimal, focused set

## 🚀 Future Enhancements

Potential improvements for production use:
- Texture loading and management
- Model loading (Assimp integration)
- Audio system
- Physics engine integration
- UI/Text rendering system
- Save/load game state
- Multiplayer support
- Advanced lighting (shadows, PBR)

## 📝 License

MIT License - feel free to use this in your portfolio or projects!

## 👨‍💻 Author

Created as a demonstration of C++ game engine development skills.

---

**Note**: This engine is designed to showcase programming ability and modern C++ practices. It demonstrates:
- Object-oriented design
- Resource management
- Graphics programming
- Game loop architecture
- Clean code principles

Perfect for showcasing to potential employers interested in C++ development!

# 3D-Game
