#pragma once

#include <vector>
#include <memory>

namespace GLRenderer {
    class IShader;
    class ITexture;
    struct Vertex;

    class IMesh {
    public:
        virtual ~IMesh() = default;

        virtual void Draw(IShader& shader) = 0;

        virtual const std::vector<Vertex>& GetVertices() const = 0;
        virtual const std::vector<unsigned int>& GetIndices() const = 0;
        virtual const std::vector<std::shared_ptr<ITexture>>& GetTextures() const = 0;
    };

}
