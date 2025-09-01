# BlockGame

A Minecraft-inspired voxel engine built from scratch using C++, SFML, and OpenGL. This project demonstrates advanced graphics programming, multithreaded architecture, and real-time 3D world generation.

![Screenshot Placeholder]

## 🎯 Project Overview

BlockGame is a solo-developed voxel engine that recreates the core mechanics of Minecraft while exploring modern game engine architecture patterns. The project emphasizes performance optimization, clean code architecture, and real-time world streaming.

## 🔧 Technology Stack

- **Language**: C++17/20
- **Graphics**: OpenGL 3.3+ Core Profile
- **Windowing & Input**: SFML 2.6
- **Mathematics**: GLM (OpenGL Mathematics)
- **Build System**: CMake with vcpkg package management
- **Development Environment**: CLion with MSVC compiler
- **Project Management**: Jira with Kanban methodology

## 🎮 Features Implemented

### **Core Rendering System**
- ✅ **Modern OpenGL 3.3+ Core Profile** - Efficient mesh rendering using VAOs, VBOs, and EBOs
- ✅ **Custom GLSL Shaders** - Vertex and fragment shaders for block texturing and lighting
- ✅ **Texture Atlas System** - Optimized batch rendering of multiple block types
- ✅ **3D Block World Rendering** - Real-time voxel visualization with proper depth testing

### **World Generation & Management**
- ✅ **Procedural Terrain Generation** - Custom noise-based algorithms for realistic landscapes
- ✅ **Cubic Chunk System** - 32³ block chunks for efficient world partitioning
- ✅ **Infinite World Streaming** - Dynamic loading and unloading of world sections
- ✅ **Memory-Optimized Chunk Storage** - Smart pointer management preventing memory leaks

### **Performance & Threading**
- ✅ **Multithreaded Architecture** - Separate threads for world generation and rendering
- ✅ **Thread Pool Implementation** - Custom ThreadManager using std::jthread for async operations
- ✅ **Thread-Safe Data Structures** - Lock-free programming with atomic operations
- ✅ **Frustum Culling** - Camera-based visibility optimization reducing unnecessary draws

### **Architecture & Tools**
- ✅ **State Management System** - Modular design for game states (menu, gameplay, settings)
- ✅ **Manager Pattern Implementation** - Separate managers for threading, world, and rendering
- ✅ **Cross-Platform Build System** - CMake with vcpkg dependency management
- ✅ **Debug & Release Optimization** - Compiler optimization balancing performance and stability
- ✅ **RenderDoc Integration** - GPU debugging and performance profiling capabilities

### **Controls & Interaction**
- ✅ **First-Person Camera System** - Smooth mouse look and WASD movement controls
- ✅ **Real-Time Input Handling** - Responsive player movement using SFML input system

## 📊 Project Management

This project was developed using Agile methodology with Jira for task management:

![Jira Kanban Board Screenshot]

## 🎓 Learning Resources

During development, I primarily referenced:
- **LearnOpenGL.com** - Modern OpenGL tutorials and best practices
- **SFML Documentation** - Window management and input handling
- **Victor Gordan (YouTube)** - OpenGL optimization techniques
- **WSAL Evan (YouTube)** - Developing feature roadmap

