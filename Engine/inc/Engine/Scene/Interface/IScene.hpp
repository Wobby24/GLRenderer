#pragma once

#include <Renderer/Interface/ModelInstance.hpp>
#include <Platform/Input/Interface/IInputService.hpp>
#include <Renderer/Interface/IRenderScene.hpp>
#include <vector>

namespace VR_Engine {
    namespace Scene {
        // Scene interface
        class IScene {
        public:
            virtual ~IScene() = default;

            // Add a model + transform instance to the scene
            virtual void addModelInstance(const Renderer::ModelInstance& instance) = 0;

            virtual void setRenderScene(std::unique_ptr<Renderer::IRenderScene> rs) = 0;

            virtual const Renderer::IRenderScene* getRenderScene() const = 0;

            // Get all model instances for the renderer
            virtual const std::vector<Renderer::ModelInstance>& getModelInstances() const = 0;

            // Update scene logic (animations, etc.)
            virtual void update(float deltaTime) = 0;

            virtual void handleInput(Platform::Input::IInputService* input);

        };
    }
}