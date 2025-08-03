#pragma once

#include <glad/glad.h> // or your loader
#include <GLFW/glfw3.h>
#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/Interface/Types/Vertex.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/OpenGL/GLMeshBuffers.hpp>
#include <iostream>

namespace GLRenderer {
	class TexturedQuad : public IRenderScene {
	public:
		TexturedQuad();
		~TexturedQuad() override;
		void Init() override;
		void Update(float deltaTime) override;
		void Render() override;
		void Cleanup() override;
		// Return a descriptor identifying this scene
		SceneDescriptor GetSceneDescriptor() const override {
			return SceneDescriptor{ SceneType::TexturedQuad, 0 };
		}
	private:
		void SetupBuffers();

		//Indices but in a vector
		std::vector<unsigned int> vIndices_ = {
			0, 1, 3, //first triangles
			1, 2, 3 //second triangle
		};


		std::vector<Vertex> vertexData_ = {
			{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
			{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } }
		};


		//We can now use GLMeshBuffers for centralized mesh control
		GLMeshBuffers meshBuffer_;
		//shader class that handles external shader files aswell as teh shaders and the program themselves/itself
		GLShader mainShader_;
		//2D image texture 
		GLTexture2D texture_;
		//2D image texture 2: electric boogaloo
		GLTexture2D texture2_;

		// Initialization, cleanup, and wireframe flags
		bool isInitialized_ = false;
		bool isCleaned_ = false;
		bool isWireframe_ = false;

	};
}

