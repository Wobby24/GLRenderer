#pragma once

#include <Renderer/Interface/IRenderScene.hpp>

// OpenGL rendering
#include <Renderer/OpenGL/GLShader.hpp>
#include <Renderer/OpenGL/GLMeshBuffers.hpp>
#include <Renderer/OpenGL/TransformableGLModel.hpp>

// Lighting
#include <Renderer/OpenGL/Lighting/GLLightManager.hpp>
#include <Renderer/OpenGL/Lighting/GLMaterial.hpp>

// Camera
#include <Renderer/OpenGL/GLCamera.hpp>

// Types
#include <Renderer/Interface/Types/Vertex.hpp>

// STL
#include <memory>
#include <unordered_map>
#include <vector>

namespace Renderer {
    namespace OpenGL {

        class ModelLoading final : public IRenderScene {
        public:
            ModelLoading();
            ~ModelLoading() override;

            // IRenderScene
            void Init() override;
            void Update(float deltaTime) override;   // animations only
            void Render() override;                  // draw calls + ImGui widgets
            void Cleanup() override;

            SceneDescriptor GetSceneDescriptor() const override {
                return { SceneType::Model3D, 0 };
            }

            void OnSurfaceResize(int width, int height) override;

        private:
            // -----------------------------
            // Resource setup
            // -----------------------------
            void initResources();
            void setupModel();
            void setupBuffers();
            void initCamera();

            // -----------------------------
            // Rendering helpers
            // -----------------------------
            void renderScene();
            void renderLighting();
            void renderModels();

            // -----------------------------
            // ImGui layout (NO backend init)
            // -----------------------------
            void renderLightingUI();
            void renderModelUI();
            void drawMaterialEditor(GLMaterial& material);
            bool drawVec3Control(
                const std::string& label,
                glm::vec3& values,
                float resetValue = 0.0f,
                bool isColor = false
            );

        private:
            // -----------------------------
            // GPU resources
            // -----------------------------
            GLShader lightingShader_;
            GLShader lightSourceShader_;
            GLMeshBuffers lightMesh_;

            std::unordered_map<int, std::shared_ptr<TransformableGLModel>> models_;
            int nextModelID_ = 0;

            // -----------------------------
            // Camera & lighting
            // -----------------------------
            std::unique_ptr<GLCamera> camera_;
            std::unique_ptr<GLLightManager> lightManager_;

            // -----------------------------
            // Matrices
            // -----------------------------
            glm::mat4 view_{ 1.0f };
            glm::mat4 projection_{ 1.0f };

            // -----------------------------
            // Geometry
            // -----------------------------
            std::vector<Vertex> cubeVertices_;

            // -----------------------------
            // State
            // -----------------------------
            bool initialized_ = false;
            bool wireframe_ = false;
        };

    } // namespace OpenGL
} // namespace Renderer
