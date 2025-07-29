#pragma once

#include <vector>
#include "GLRenderer/Interface/Vertex.hpp"
#include "GLRenderer/Interface/IMeshBuffers.hpp"

namespace GLRenderer {

	class GLMeshBuffers : public IMeshBuffers {
	public:
		GLMeshBuffers();
		~GLMeshBuffers() override;

		void CreateAll(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, int attributes = 0) override;
		void CreateVertices(const std::vector<Vertex>& vertices, int attributes = 0) override;
		void CreateIndices(const std::vector<unsigned int>& indices) override;

		void Bind() const override;
		void Unbind() const override;

	private:
		void EnableAttributes(int attributes);

		unsigned int VAO_ = 0;
		unsigned int VBO_ = 0;
		unsigned int EBO_ = 0;

		size_t indexCount_ = 0;
	};

}
