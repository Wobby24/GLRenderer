#include <Scene/VR_Test_Scene.hpp>

namespace VR_Engine {
	namespace Scene {

        void VR_Test_Scene::setRenderScene(std::unique_ptr<Renderer::IRenderScene> rs) {
            renderScene_ = std::move(rs);
        }

        void VR_Test_Scene::addModelInstance(const Renderer::ModelInstance& instance) {
            instances.push_back(instance);
        }

        const std::vector<Renderer::ModelInstance>& VR_Test_Scene::getModelInstances() const {
            return instances;
        }

        void VR_Test_Scene::update(float deltaTime) {

        }

        void VR_Test_Scene::updateRenderScene(float deltaTime) {

        }

        void VR_Test_Scene::handleInput(Platform::Input::IInputService* inputService) {

        }            
	}
}