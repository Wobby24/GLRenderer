#pragma once

#include <vector>
#include "Types/Vertex.hpp"
#include "Types/VertexAttribFlags.hpp"   // Include the enum class definition

namespace Renderer {

    class IMeshBuffers {
    public:
        virtual ~IMeshBuffers() = default;

        // Use VertexAttribFlags instead of int for type safety
        virtual void CreateAll(const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices,
            VertexAttribFlags attributes = VertexAttribFlags::NONE) = 0;

        virtual void CreateVertices(const std::vector<Vertex>& vertices,
            VertexAttribFlags attributes = VertexAttribFlags::NONE) = 0;

        virtual void CreateIndices(const std::vector<unsigned int>& indices) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void Cleanup() = 0; // Manual cleanup
    };

}
