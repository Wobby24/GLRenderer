#pragma once

#include <vector>
#include "Renderer/Interface/Types/Vertex.hpp"
#include "Renderer/Interface/IMeshBuffers.hpp"

namespace Renderer {
	namespace OpenGL {
		class GLMeshBuffers : public IMeshBuffers {
		public:
			GLMeshBuffers();
			~GLMeshBuffers() override = default;

			void CreateVertices(const std::vector<Vertex>& vertices, VertexAttribFlags attributes = VertexAttribFlags::NONE);
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
}
