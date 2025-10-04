#pragma once

#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/Window/Interface/IWindow.hpp>
#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/Interface/Types/Vertex.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/GLMeshBuffers.hpp>
#include <GLRenderer/OpenGL/GLSceneInputHandler.hpp>
#include <GLRenderer/OpenGL/Lighting/GLMaterial.hpp>
#include <GLRenderer/OpenGL/Lighting/Types/GLPointLight.hpp>
#include <GLRenderer/OpenGL/Lighting/GLLightManager.hpp>
#include <iostream>
#include <memory>

namespace GLRenderer {
	class LightingCubes : public IRenderScene {
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

		std::vector<Vertex> planeVertices_ = {
			// Bottom-left
			Vertex({ -0.5f, 0.0f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }),
			// Bottom-right
			Vertex({  0.5f, 0.0f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }),
			// Top-right
			Vertex({  0.5f, 0.0f,  0.5f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }),
			// Top-left
			Vertex({ -0.5f, 0.0f,  0.5f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }),
		};

		std::vector<uint32_t> planeIndices_ = {
			0, 1, 2, // First triangle
			2, 3, 0  // Second triangle
		};

		const glm::vec3 cubePositions[10] = {
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


		//input handler for camera and window callbacks 
		std::unique_ptr<GLSceneInputHandler> inputHandler_;
		std::unique_ptr<GLCamera> camera_;
		std::unique_ptr<GLLightManager> lightManager_;

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
		GLMeshBuffers planeMesh_;
		//shader class that handles external shader files aswell as teh shaders and the program themselves/itself
		GLShader lightingShader_;
		GLShader lightSourceShader_;
		//textures
		GLTexture2D containerDiffuse_;
		GLTexture2D containerSpecular_;
		GLTexture2D containerEmission_;
		//material for copper
		GLMaterial containerMat_;
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

