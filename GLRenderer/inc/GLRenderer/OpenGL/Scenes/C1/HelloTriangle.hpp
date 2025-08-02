#pragma once

#include <vector>
#include <glad/glad.h> // or your loader
#include <GLFW/glfw3.h>

#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/Interface/Types/Vertex.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/GLMeshBuffers.hpp>
#include <iostream>

namespace GLRenderer {

    class HelloTriangle : public IRenderScene {
    public:
        HelloTriangle();
        ~HelloTriangle() override;

        void Init() override;
        void Update(float deltaTime) override;
        void Render() override;
        void Cleanup() override;

        // Return a descriptor identifying this scene
        SceneDescriptor GetSceneDescriptor() const override {
            return SceneDescriptor{ SceneType::HelloTriangle, 0 };
        }

    private:
        void SetupBuffers();

        // Indices as vector (optional redundancy; you can pick either vector or array)
        std::vector<unsigned int> vIndices_ = {
            0, 1, 3,
            1, 2, 3
        };

        // Vertices using your Vertex struct (for colored triangle example)
        std::vector<Vertex> triVerts_ = {
            {{-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }},
            {{ 0.5f, -0.5f, 0.0f}, { 0.0f, 1.0f, 0.0f }},
            {{ 0.0f,  0.5f, 0.0f}, { 0.0f, 0.0f, 1.0f }}
        };

        GLMeshBuffers meshBuffer_;
        GLShader mainShader_;

        bool isInitialized_ = false;
        bool isCleaned_ = false;
        bool isWireframe_ = false;
    };

} // namespace GLRenderer
