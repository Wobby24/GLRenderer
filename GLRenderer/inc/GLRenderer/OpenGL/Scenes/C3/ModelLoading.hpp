#pragma once 

#include <GLRenderer/Window/Interface/IWindow.hpp>
#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/Interface/IResizableScene.hpp>
#include <GLRenderer/OpenGL/GLSceneInputHandler.hpp>
//#include <GLRenderer/OpenGL/Lighting/GLMaterial.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLPointLight.hpp>
#include <GLRenderer/OpenGL/Lighting/GLLightManager.hpp>
#include <GLRenderer/OpenGL/GLModel.hpp>
#include <GLRenderer/OpenGL/TransformableGLModel.hpp>
#include <iostream>
#include <memory>

namespace GLRenderer {
	class ModelLoading : public IResizableScene {
	public:
		ModelLoading();
		~ModelLoading() override;
		void Init() override;
		void Update(float deltaTime) override;
		void OnWindowResize(int newWidth, int newHeight) override;
		void Render() override;
		void Cleanup() override;
		void SetWindow(Window::IWindow& window) override;
		//void RenderGUI() override;
		// Return a descriptor identifying this scene
		SceneDescriptor GetSceneDescriptor() const override {
			return SceneDescriptor{ SceneType::Model3D, 0 };
		}

		// Expose input handler
		ISceneInputHandler* GetInputHandler() { return inputHandler_.get(); }
	private:
		void SetupModel();
		//input handler for camera and window callbacks 
		std::unique_ptr<GLSceneInputHandler> inputHandler_;
		std::unique_ptr<GLCamera> camera_;
		std::unique_ptr<GLLightManager> lightManager_;
		std::unique_ptr<GLModel> backpack_;
		std::unique_ptr<TransformableGLModel> model_;

		void SetupBuffers();

		std::vector<Vertex> cubeVertices_ = {
	{ { -0.5f, -0.5f, -0.5f } },
	{ {  0.5f, -0.5f, -0.5f } },
	{ {  0.5f,  0.5f, -0.5f } },
	{ {  0.5f,  0.5f, -0.5f } },
	{ { -0.5f,  0.5f, -0.5f } },
	{ { -0.5f, -0.5f, -0.5f } },

	{ { -0.5f, -0.5f,  0.5f } },
	{ {  0.5f, -0.5f,  0.5f } },
	{ {  0.5f,  0.5f,  0.5f } },
	{ {  0.5f,  0.5f,  0.5f } },
	{ { -0.5f,  0.5f,  0.5f } },
	{ { -0.5f, -0.5f,  0.5f } },

	{ { -0.5f,  0.5f,  0.5f } },
	{ { -0.5f,  0.5f, -0.5f } },
	{ { -0.5f, -0.5f, -0.5f } },
	{ { -0.5f, -0.5f, -0.5f } },
	{ { -0.5f, -0.5f,  0.5f } },
	{ { -0.5f,  0.5f,  0.5f } },

	{ {  0.5f,  0.5f,  0.5f } },
	{ {  0.5f,  0.5f, -0.5f } },
	{ {  0.5f, -0.5f, -0.5f } },
	{ {  0.5f, -0.5f, -0.5f } },
	{ {  0.5f, -0.5f,  0.5f } },
	{ {  0.5f,  0.5f,  0.5f } },

	{ { -0.5f, -0.5f, -0.5f } },
	{ {  0.5f, -0.5f, -0.5f } },
	{ {  0.5f, -0.5f,  0.5f } },
	{ {  0.5f, -0.5f,  0.5f } },
	{ { -0.5f, -0.5f,  0.5f } },
	{ { -0.5f, -0.5f, -0.5f } },

	{ { -0.5f,  0.5f, -0.5f } },
	{ {  0.5f,  0.5f, -0.5f } },
	{ {  0.5f,  0.5f,  0.5f } },
	{ {  0.5f,  0.5f,  0.5f } },
	{ { -0.5f,  0.5f,  0.5f } },
	{ { -0.5f,  0.5f, -0.5f } }
		};
		//mvp matrices
		//glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view_ = glm::mat4(1.0);
		glm::mat4 projection_ = glm::mat4(1.0);

		//shader class that handles external shader files aswell as teh shaders and the program themselves/itself
		GLShader lightingShader_;
		GLShader lightSourceShader_;
		GLMeshBuffers lightMesh_;
		//context for camera input
		GLCameraInputContext inputContext_;

		// Initialization, cleanup, and wireframe flags
		bool isInitialized_ = false;
		bool isCleaned_ = false;
		bool isWireframe_ = false;
		bool imguiInitialized_ = false;
		bool isPointerLocked_ = true;
		bool showExitConfirmDialog = false;

		//proj matrix stuff
		//also this is just set to a compatible res, not anything specific
		int windowWidth_ = 1280;
		int windowHeight_ = 720;

		void initResources();
		void initCamera();
		void updateMovement(float deltatime);
		void initImGUI(GLFWwindow* window);
		void cleanupImGUI();
		void renderLightingUI();
		//void reloadResources();
		void renderImGUI();
	};
}