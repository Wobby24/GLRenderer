#include <glad/glad.h> // or your loader
#include <GLFW/glfw3.h>
#include <GLRenderer/Interface/IRenderScene.hpp>
#include <GLRenderer/Interface/Vertex.hpp>
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
		SceneID GetSceneID() const override { return SceneID::HelloTriangle; }
	private:
		void SetupShaders();
		void SetupVertexShader();
		void SetupFragmentShader();
		void SetupShaderProgram();
		void SetupBuffers();
		// Vertex data for a triangle
		float vertices_[12] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};
		// Indices for the triangle (two triangles forming a rectangle)
		unsigned int indices_[6] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		//Indices but in a vector
		std::vector<unsigned int> vIndices_ = {
			0, 1, 3, //first triange
			1, 2, 3 //second triangle
		};

		//vertices using our Vertex struct
		std::vector<Vertex> triVerts_ = {
			{{-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }},
			{{ 0.5f, -0.5f, 0.0f}, { 0.0f, 1.0f, 0.0f }},
			{{ 0.0f,  0.5f, 0.0f}, { 0.0f, 0.0f, 1.0f }}
		};

		//We can now use GLMeshBuffers for centralized mesh control
		GLMeshBuffers meshBuffer_;

		// Initialization, cleanup, and wireframe flags
		bool isInitialized_ = false;
		bool isCleaned_ = false;
		bool isWireframe_ = false;

		//vertex and fragment shader ID, as-well as shader program ID
		unsigned int vertexShader_, fragmentShader_, shaderProgram_;

		//Vertex shader code
		const char* vertexShaderSource = "#version 460 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"out vec3 ourColor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"   ourColor = aColor;\n"
			"}\0";

		// Fragment shader code
		const char* fragmentShaderSource = "#version 460 core\n"
			"out vec4 FragColor;\n"
			"in vec3 ourColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(ourColor, 1.0);\n"
			"}\0";
	};
}

