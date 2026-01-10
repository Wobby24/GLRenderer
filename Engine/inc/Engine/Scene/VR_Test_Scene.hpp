#pragma once

#include <Scene/Interface/IScene.hpp>
#include <Renderer/Interface/IRenderScene.hpp>
#include <Renderer/Interface/ModelInstance.hpp>
#include <Platform/Input/Interface/IInputService.hpp>
#include <Renderer/Interface/ICamera.hpp>
#include <vector>

namespace VR_Engine {
	namespace Scene {
		class VR_Test_Scene : public IScene {
		public:

			VR_Test_Scene() = default;
            ~VR_Test_Scene() = default;

			void addModelInstance(const Renderer::ModelInstance& instance) override;

			void setRenderScene(std::unique_ptr<Renderer::IRenderScene> rs) override;

			const Renderer::IRenderScene* getRenderScene() const { return renderScene_.get(); }

			const std::vector<Renderer::ModelInstance>& getModelInstances() const override;

            void update(float deltaTime) override;

			void handleInput(Platform::Input::IInputService* inputService) override;

		private:
			void updateRenderScene(float deltaTime);

			std::unique_ptr<Renderer::IRenderScene> renderScene_;
			std::vector<Renderer::ModelInstance> instances;

			// Render Scene data:
			std::unique_ptr<Renderer::ICamera> camera_;
			
		};
	}
}