#include <GLRenderer/OpenGL/GLMesh.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/Interface/Types/VertexAttribFlagsOperators.hpp>
#include <glad/glad.h>

namespace GLRenderer {
    GLMesh::GLMesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<std::shared_ptr<ITexture>>& textures) : isInit_(false), isCleanedUp_(false)
    {
        vertices_ = vertices;
        indices_ = indices;
        textures_ = textures;
    }

    GLMesh::GLMesh(const std::vector<Vertex>& vertices,
        const std::vector<std::shared_ptr<ITexture>>& textures)
        : isInit_(false), isCleanedUp_(false),
        vertices_(vertices), textures_(textures)
    {
    }

    GLMesh::GLMesh(const std::vector<Vertex>& vertices)
        : isInit_(false), isCleanedUp_(false),
        vertices_(vertices)
    {
    }

    GLMesh::~GLMesh() {
        Cleanup(); // Safe, it's idempotent
    }

    void GLMesh::Init() {
        if (isInit_) return;
        setupMesh();
        isInit_ = true;
    }

    void GLMesh::Cleanup() {
        if (isCleanedUp_ == true) return;

        meshBuffers_.Cleanup();

        isCleanedUp_ = true;
        isInit_ = false;
    }

    void GLMesh::setupMesh() {

        meshBuffers_.Bind();

        if (!vertices_.empty() && !indices_.empty()) {
            meshBuffers_.CreateAll(vertices_, indices_, GLRenderer::VertexAttribFlags::POSITION | GLRenderer::VertexAttribFlags::TEXCOORDS | GLRenderer::VertexAttribFlags::NORMAL);
        }
        else if (!vertices_.empty()) {
            meshBuffers_.CreateVertices(vertices_, GLRenderer::VertexAttribFlags::POSITION | GLRenderer::VertexAttribFlags::TEXCOORDS | GLRenderer::VertexAttribFlags::NORMAL);
        }
        else {
            // Optional: log error or assert
            std::cerr << "GLMesh::setupMesh() - No vertex data provided!" << std::endl;
            return;
        }  

        meshBuffers_.Unbind();
    }

    void GLMesh::Draw(IShader& shaderBase) {
        // Cast to GLShader
        GLShader* shader = dynamic_cast<GLShader*>(&shaderBase);
        if (!shader) {
            std::cerr << "Shader is not a GLShader.\n";
            return;
        }

        unsigned int diffuseIndex = 0;
        unsigned int specularIndex = 0;
        unsigned int emissionIndex = 0;

        for (unsigned int i = 0; i < textures_.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);

            // Cast to GLTexture2D
            GLTexture2D* glTex = dynamic_cast<GLTexture2D*>(textures_[i].get());
            if (!glTex) {
                std::cerr << "Texture is not a GLTexture2D.\n";
                continue;
            }

            TextureType type = glTex->getType();

            std::string uniformName;
            switch (type) {
            case TextureType::DIFFUSE:
                if (diffuseIndex >= 8) continue;
                uniformName = "material.diffuse[" + std::to_string(diffuseIndex) + "]";
                shader->setInt(uniformName, i);
                uniformName = "useDiffuseMap";
                shader->setBool(uniformName, true);
                diffuseIndex++;
                break;

            case TextureType::SPECULAR:
                if (specularIndex >= 8) continue;
                uniformName = "material.specular[" + std::to_string(specularIndex) + "]";
                shader->setInt(uniformName, i);
                uniformName = "useSpecularMap";
                shader->setBool(uniformName, true);
                specularIndex++;
                break;

            case TextureType::EMISSION:
                if (emissionIndex >= 8) continue;
                uniformName = "material.emission[" + std::to_string(emissionIndex) + "]";
                shader->setInt(uniformName, i);
                uniformName = "useEmissionMap";
                shader->setBool(uniformName, true);
                emissionIndex++;
                break;

            default:
                // Unknown or unsupported texture type, skip binding
                continue;
            }

            // Bind the texture to the current active texture unit
            glTex->bind(i);
        }

        // Set the counts to the shader (make sure your shader has these uniforms!)
        shader->setInt("numDiffuseTextures", diffuseIndex);
        shader->setInt("numSpecularTextures", specularIndex);
        shader->setInt("numEmissionTextures", emissionIndex);

        // Draw
        glActiveTexture(GL_TEXTURE0); // reset active texture
        meshBuffers_.Bind();
        if (!indices_.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
        }
        else
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size()));
        meshBuffers_.Unbind();
    }

}
