#pragma once

#include <vector>
#include "Vertex.hpp"

namespace GLRenderer {
    enum VertexAttribFlags {
        POSITION = 1 << 0,
        TEXCOORD = 1 << 1,
        NORMAL = 1 << 2
    };

    class IMeshBuffers {
    public:
        virtual ~IMeshBuffers() = default;

        virtual void CreateAll(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, int attributes = 0) = 0;
        virtual void CreateVertices(const std::vector<Vertex>& vertices, int attributes = 0) = 0;
        virtual void CreateIndices(const std::vector<unsigned int>& indices) = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}

