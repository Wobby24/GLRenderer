#include <glad/glad.h> // or your loader
#include <GLFW/glfw3.h>
#include <GlRenderer/Interface/IRenderScene.hpp>
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
		float tri_vertices_[9] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
		};
		// Vertex Buffer Object and Vertex Array Object, Don't use the GLMeshBuffers just quite yet
		unsigned int VBO_, VAO_, EBO_;

		// Initialization, cleanup, and wireframe flags
		bool isInitialized_ = false;
		bool isCleaned_ = false;
		bool isWireframe_ = false;

		//vertex and fragment shader ID, as-well as shader program ID
		unsigned int vertexShader, fragmentShader, shaderProgram;

		//Vertex shader code
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"out vec4 vertexColor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // Set a color for the vertex\n"
			"}\0";

		// Fragment shader code
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec4 vertexColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vertexColor;\n"
			"}\0";
	};
}

