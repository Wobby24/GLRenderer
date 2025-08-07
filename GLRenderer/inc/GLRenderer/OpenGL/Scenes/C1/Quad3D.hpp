#pragma once

#include <glad/glad.h> // or your loader
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/Window/Interface/IWindow.hpp>
#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/Interface/Types/Vertex.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/GLMeshBuffers.hpp>
#include <GLRenderer/Interface/IResizableScene.hpp>
//#include <GLRenderer/Interface/IGUIRenderScene.hpp>
#include <GLRenderer/OpenGL/GLSceneInputHandler.hpp>
#include <iostream>
#include <memory>

namespace GLRenderer {
	class Quad3D : public IResizableScene {
	public:
		Quad3D();
		~Quad3D() override;
		void Init() override;
		void Update(float deltaTime) override;
		void OnWindowResize(int newWidth, int newHeight) override;
		void Render() override;
		void Cleanup() override;
		void SetWindow(Window::IWindow& window) override;
		//void RenderGUI() override;
		// Return a descriptor identifying this scene
		SceneDescriptor GetSceneDescriptor() const override {
			return SceneDescriptor{ SceneType::Quad3D, 0 };
		}

		// Expose input handler
		ISceneInputHandler* GetInputHandler() { return inputHandler_.get(); }
	private:
		void SetupBuffers();

		std::vector<Vertex> cubeVertices_ = {
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },
	{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } },
	{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
	{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
	{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },

	{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
	{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f } },
	{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f } },
	{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f } },
	{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },

	{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
	{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
	{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },

	{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
	{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
	{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
	{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
	{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },

	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },
	{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } },
	{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } },

	{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
	{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
	{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } },
	{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } },
	{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } }
		};

		// world space positions of our cubes
		glm::vec3 cubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		std::vector<float> cubeAngles_; // Add this to your class (initialize with 10 elements)

		//input handler for camera and window callbacks 
		std::unique_ptr<GLSceneInputHandler> inputHandler_;
		std::unique_ptr<GLCamera> camera_;

		//mvp matrices
		//glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);

		//We can now use GLMeshBuffers for centralized mesh control
		GLMeshBuffers meshBuffer_;
		//shader class that handles external shader files aswell as teh shaders and the program themselves/itself
		GLShader mainShader_;
		//2D image texture 
		GLTexture2D texture_;
		//2D image texture 2: electric boogaloo
		GLTexture2D texture2_;
		//context for camera input
		GLCameraInputContext inputContext_;

		// Initialization, cleanup, and wireframe flags
		bool isInitialized_ = false;
		bool isCleaned_ = false;
		bool isWireframe_ = false;

		//proj matrix stuff
		int windowWidth_ = 1280;
		int windowHeight_ = 720;

		void initResources();
		void initCamera();
		void updateMovement(float deltatime);
	};
}

