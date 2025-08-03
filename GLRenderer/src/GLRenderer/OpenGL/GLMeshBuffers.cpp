#include <GLRenderer/OpenGL/GLMeshBuffers.hpp>
#include <GLRenderer/Interface/Types/VertexAttribFlags.hpp>
#include <GLRenderer/Interface/Types/VertexAttribFlagsOperators.hpp>
#include <glad/glad.h>

namespace GLRenderer {

	//destructor is default since we need explicit control over the mesh buffers

	GLMeshBuffers::GLMeshBuffers() : VAO_(0), VBO_(0), EBO_(0), indexCount_(0) {}

	void GLMeshBuffers::CreateAll(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, VertexAttribFlags attributes) {
		CreateVertices(vertices, attributes);
		CreateIndices(indices);
	}

	void GLMeshBuffers::CreateVertices(const std::vector<Vertex>& vertices, VertexAttribFlags attributes) {
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

	void GLMeshBuffers::EnableAttributes(VertexAttribFlags attributes) {

		if (HasFlag(attributes, VertexAttribFlags::POSITION)) {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(0);
		}

		if (HasFlag(attributes, VertexAttribFlags::TEXCOORDS)) {
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			glEnableVertexAttribArray(1);
		} 

		if (HasFlag(attributes, VertexAttribFlags::COLOR)) {
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
			glEnableVertexAttribArray(2);
		}
	//	if (attributes & NORMAL) {
		//	glEnableVertexAttribArray(3);
		//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//	}
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