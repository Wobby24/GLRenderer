#pragma once

#include <vector>
#include "GLRenderer/Interface/Types/Vertex.hpp"
#include "GLRenderer/Interface/IMeshBuffers.hpp"

namespace GLRenderer {

	class GLMeshBuffers : public IMeshBuffers {
	public:
		GLMeshBuffers();
		~GLMeshBuffers() override = default;

		void CreateVertices(const std::vector<Vertex>& vertices, VertexAttribFlags attributes = VertexAttribFlags::NONE) ;
		void CreateAll(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, VertexAttribFlags attributes = VertexAttribFlags::NONE) override;
		void CreateIndices(const std::vector<unsigned int>& indices) override;

		void Bind() const override;
		void Unbind() const override;

		void Cleanup() override;

	private:
		void EnableAttributes(VertexAttribFlags attributes);

		unsigned int VAO_ = 0;
		unsigned int VBO_ = 0;
		unsigned int EBO_ = 0;

		size_t indexCount_ = 0;
	};

}
