#pragma once

#include <glad/glad.h> // or your loader
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/Window/Interface/IWindow.hpp>
#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/Interface/Types/Vertex.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/GLMeshBuffers.hpp>
#include <GLRenderer/Interface/IResizableScene.hpp>
#include <GLRenderer/OpenGL/GLSceneInputHandler.hpp>
#include <GLRenderer/OpenGL/Lighting/GLMaterial.hpp>
#include <GLRenderer/OpenGL/Lighting/GLPointLight.hpp>
#include <iostream>
#include <memory>

namespace GLRenderer {
	class LightingCubes : public IResizableScene {
	public:
		LightingCubes();
		~LightingCubes() override;
		void Init() override;
		void Update(float deltaTime) override;
		void OnWindowResize(int newWidth, int newHeight) override;
		void Render() override;
		void Cleanup() override;
		void SetWindow(Window::IWindow& window) override;
		//void RenderGUI() override;
		// Return a descriptor identifying this scene
		SceneDescriptor GetSceneDescriptor() const override {
			return SceneDescriptor{ SceneType::LightingCubes, 0 };
		}

		// Expose input handler
		ISceneInputHandler* GetInputHandler() { return inputHandler_.get(); }
	private:
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

		std::vector<Vertex> verticesNorm_ = {
	{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},
	{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}},

	{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f, 1.0f}},

	{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}},

	{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}},

	{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
	{{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}},

	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}},
		};
 
		std::vector<Vertex> texNormVertices = {
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},

	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 0.0f,  0.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f,  0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, { 0.0f,  0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, { 0.0f,  0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, { 0.0f,  0.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 0.0f,  0.0f, 1.0f}},

	{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},

	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},

	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},

	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f,  1.0f,   0.0f}}
		};


		//input handler for camera and window callbacks 
		std::unique_ptr<GLSceneInputHandler> inputHandler_;
		std::unique_ptr<GLCamera> camera_;

		//mvp matrices
		//glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view_ = glm::mat4(1.0);
		glm::mat4 projection_ = glm::mat4(1.0);

		//light positions and colors
		glm::vec3 lightPos_ = glm::vec3(1.2f, 1.0f, 2.0f);
		glm::vec3 lightColor_ = glm::vec3(1.0f, 1.0f, 1.0f);
		//glm::vec3 objectColor_ = glm::vec3(1.0f, 0.5f, 0.31f);

		//We can now use GLMeshBuffers for centralized mesh control
		GLMeshBuffers cubeMesh_;
		GLMeshBuffers lightMesh_;
		//shader class that handles external shader files aswell as teh shaders and the program themselves/itself
		GLShader lightingShader_;
		GLShader lightSourceShader_;
		GLTexture2D copper_;
		//material for copper
		GLMaterial copperMat_;
		//point-light
		GLPointLight pointLight_;
		//context for camera input
		GLCameraInputContext inputContext_;

		// Initialization, cleanup, and wireframe flags
		bool isInitialized_ = false;
		bool isCleaned_ = false;
		bool isWireframe_ = false;
		bool imguiInitialized_ = false;

		//proj matrix stuff
		//also this is just set to a compatible res, not anything specific
		int windowWidth_ = 1280;
		int windowHeight_ = 720;

		void initResources();
		void initCamera();
		void updateMovement(float deltatime);
		void initImGUI(GLFWwindow* window);
		void cleanupImGUI();
		//void reloadResources();
		void renderImGUI();
	};
}

