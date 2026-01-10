#pragma once

#include "Renderer/Interface/IMesh.hpp"
#include "Lighting/GLMaterial.hpp"
#include "GLMeshBuffers.hpp"
#include "GLTextureBinder.hpp"
#include "GLShader.hpp"   // Implements IShader

#include <glad/glad.h>
#include <memory>
#include <string>
#include <vector>

namespace Renderer {
    namespace OpenGL {
        class GLMesh : public IMesh {
        public:
            // Constructor with vertices, indices, and material
            GLMesh(const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices,
                std::shared_ptr<GLMaterial> material);

            // Constructor with vertices and material (no indices)
            GLMesh(const std::vector<Vertex>& vertices,
                std::shared_ptr<GLMaterial> material);

            // Legacy constructors removed or deprecated (since material is mandatory now)
            // If you want to keep the old ones, you can mark them deprecated or remove them

            ~GLMesh();

            void Init();              // Creates VAO, VBO, EBO
            void Cleanup();           // Deletes GL resources
            void Draw(IShader& shader) override;
            void Draw(IShader& shaderBase, GLTextureBinder& binder);

            const std::vector<Vertex>& GetVertices() const override { return vertices_; }
            const std::vector<unsigned int>& GetIndices() const override { return indices_; }

            void DrawWithoutBinding() { drawWithoutBinding(); }
            void BindMaterialTextures(IShader& shaderBase, GLTextureBinder& binder) { bindMaterialTextures(shaderBase, binder); }

            // Since textures are handled by material now, this returns empty or can be removed
            const std::vector<std::shared_ptr<ITexture>>& GetTextures() const override {
                static const std::vector<std::shared_ptr<ITexture>> emptyTextures;
                return emptyTextures;
            }

            // Provide access to material if needed
            std::shared_ptr<GLMaterial> GetMaterial() const { return material_; }

        private:
            std::vector<Vertex> vertices_;
            std::vector<unsigned int> indices_;

            std::shared_ptr<GLMaterial> material_;

            GLMeshBuffers meshBuffers_;

            bool isInit_ = false;
            bool isCleanedUp_ = false;

            void setupMesh();
            void drawWithoutBinding();
            void bindMaterialTextures(IShader& shaderBase, GLTextureBinder& binder);
        };
    }
}
