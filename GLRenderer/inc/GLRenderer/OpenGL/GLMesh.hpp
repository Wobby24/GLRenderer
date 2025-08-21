#pragma once
#include "GLRenderer/Interface/IMesh.hpp"
#include "GLRenderer/Interface/ITexture.hpp"
#include "GLMeshBuffers.hpp"
#include "GLShader.hpp"   // Implements IShader

#include <glad/glad.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace GLRenderer {
    class GLMesh : public IMesh {
    public:
        GLMesh(const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices,
            const std::vector<std::shared_ptr<ITexture>>& textures);

        GLMesh(const std::vector<Vertex>& vertices, 
            const std::vector<std::shared_ptr<ITexture>>& textures);
       
        GLMesh(const std::vector<Vertex>& vertices);

        ~GLMesh();

        void Init();              // Creates VAO, VBO, EBO
        void Cleanup();           // Deletes GL resources
        void Draw(IShader& shader) override;

        const std::vector<Vertex>& GetVertices() const override { return vertices_; }
        const std::vector<unsigned int>& GetIndices() const override { return indices_; }
        const std::vector<std::shared_ptr<ITexture>>& GetTextures() const override { return textures_; }

    private:
        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;
        std::vector<std::shared_ptr<ITexture>> textures_;

        GLMeshBuffers meshBuffers_;

        bool isInit_ = false;
        bool isCleanedUp_ = false;

       // std::unordered_map<std::string, int> textureUnitCache_;

        void setupMesh();
    };
}


