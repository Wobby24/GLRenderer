#include <GLRenderer/OpenGL/GLMeshBuffers.hpp>
#include <glad/glad.h>

namespace GLRenderer {

	//destructor is default since we need explicit control over the mesh buffers

	GLMeshBuffers::GLMeshBuffers() : VAO_(0), VBO_(0), EBO_(0), indexCount_(0) {}

	void GLMeshBuffers::CreateAll(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, int attributes) {
		CreateVertices(vertices, attributes);
		CreateIndices(indices);
	}

	void GLMeshBuffers::CreateVertices(const std::vector<Vertex>& vertices, int attributes) {
		glGenVertexArrays(1, &VAO_);
		glGenBuffers(1, &VBO_);

		glBindVertexArray(VAO_);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		EnableAttributes(attributes);
		glBindVertexArray(0);
	}

	void GLMeshBuffers::CreateIndices(const std::vector<unsigned int>& indices) {
		indexCount_ = indices.size();

		glBindVertexArray(VAO_);
		glGenBuffers(1, &EBO_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	void GLMeshBuffers::EnableAttributes(int attributes) {
		if (attributes & POSITION) {
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		}

		//if (attributes )
	/*	if (attributes & TEXCOORD) {
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		}
		if (attributes & NORMAL) {
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		}*/
	}

	void GLMeshBuffers::Cleanup() {
		if (EBO_) glDeleteBuffers(1, &EBO_);
		if (VBO_) glDeleteBuffers(1, &VBO_);
		if (VAO_) glDeleteBuffers(1, &VAO_);
	}

	void GLMeshBuffers::Bind() const {
		glBindVertexArray(VAO_);
	}

	void GLMeshBuffers::Unbind() const {
		glBindVertexArray(0);
	}
}