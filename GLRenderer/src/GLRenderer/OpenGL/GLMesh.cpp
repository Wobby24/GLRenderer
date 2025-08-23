#include <GLRenderer/OpenGL/GLMesh.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/Interface/Types/VertexAttribFlagsOperators.hpp>
#include <glad/glad.h>
#include <iostream>
#include <chrono>

namespace GLRenderer {

    GLMesh::GLMesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        std::shared_ptr<GLMaterial> material)
        : vertices_(vertices), indices_(indices), material_(std::move(material)),
        isInit_(false), isCleanedUp_(false) {
    }

    GLMesh::GLMesh(const std::vector<Vertex>& vertices,
        std::shared_ptr<GLMaterial> material)
        : vertices_(vertices), material_(std::move(material)),
        isInit_(false), isCleanedUp_(false) {
    }

    GLMesh::~GLMesh() {
        Cleanup(); // Safe to call multiple times
    }

    void GLMesh::Init() {
        if (isInit_) return;
        setupMesh();
        isInit_ = true;
    }

    void GLMesh::Cleanup() {
        if (isCleanedUp_) return;
        meshBuffers_.Cleanup();
        isCleanedUp_ = true;
        isInit_ = false;
    }

    void GLMesh::setupMesh() {
        meshBuffers_.Bind();

        if (!vertices_.empty() && !indices_.empty()) {
            meshBuffers_.CreateAll(vertices_, indices_,
                GLRenderer::VertexAttribFlags::POSITION |
                GLRenderer::VertexAttribFlags::TEXCOORDS |
                GLRenderer::VertexAttribFlags::NORMAL);
        }
        else if (!vertices_.empty()) {
            meshBuffers_.CreateVertices(vertices_,
                GLRenderer::VertexAttribFlags::POSITION |
                GLRenderer::VertexAttribFlags::TEXCOORDS |
                GLRenderer::VertexAttribFlags::NORMAL);
        }
        else {
            std::cerr << "GLMesh::setupMesh() - No vertex data provided!" << std::endl;
        }

        meshBuffers_.Unbind();
    }

    void GLMesh::Draw(IShader& shaderBase) {
        GLTextureBinder binder;
        Draw(shaderBase, binder);
    }

    void GLMesh::Draw(IShader& shaderBase, GLTextureBinder& binder) {
        bindMaterialTextures(shaderBase, binder);
        drawWithoutBinding();
    }


    void GLMesh::drawWithoutBinding() {
        meshBuffers_.Bind();
        if (!indices_.empty()) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size()));
        }
        meshBuffers_.Unbind();
    }

    void GLMesh::bindMaterialTextures(IShader& shaderBase, GLTextureBinder& binder) {
        GLShader* shader = dynamic_cast<GLShader*>(&shaderBase);
        if (!shader) {
            std::cerr << "Shader is not a GLShader.\n";
            return;
        }

        if (!material_) {
            std::cerr << "GLMesh::BindMaterialTextures called without a valid material." << std::endl;
            return;
        }

        unsigned int diffuseCount = 0, specularCount = 0, emissionCount = 0;

        const auto& diffuseMaps = material_->getDiffuseTextures();
        for (unsigned int i = 0; i < diffuseMaps.size() && diffuseCount < 8; ++i, ++diffuseCount) {
            auto glTex = std::dynamic_pointer_cast<GLTexture2D>(diffuseMaps[i]);
            if (!glTex) continue;
            binder.BindTexture(glTex, i);
            shader->setInt("material.diffuse[" + std::to_string(diffuseCount) + "]", i);
        }
        shader->setInt("numDiffuseTextures", diffuseCount);
        shader->setBool("useDiffuseMap", diffuseCount > 0);

        const auto& specularMaps = material_->getSpecularTextures();
        for (unsigned int i = 0; i < specularMaps.size() && specularCount < 8; ++i, ++specularCount) {
            auto glTex = std::dynamic_pointer_cast<GLTexture2D>(specularMaps[i]);
            if (!glTex) continue;
            binder.BindTexture(glTex, diffuseCount + i);
            shader->setInt("material.specular[" + std::to_string(specularCount) + "]", diffuseCount + i);
        }
        shader->setInt("numSpecularTextures", specularCount);
        shader->setBool("useSpecularMap", specularCount > 0);

        const auto& emissionMaps = material_->getEmissionTextures();
        for (unsigned int i = 0; i < emissionMaps.size() && emissionCount < 8; ++i, ++emissionCount) {
            auto glTex = std::dynamic_pointer_cast<GLTexture2D>(emissionMaps[i]);
            if (!glTex) continue;
            binder.BindTexture(glTex, diffuseCount + specularCount + i);
            shader->setInt("material.emission[" + std::to_string(emissionCount) + "]", diffuseCount + specularCount + i);
        }
        shader->setInt("numEmissionTextures", emissionCount);
        shader->setBool("useEmissionMap", emissionCount > 0);

        material_->applyProperties(shaderBase);

        glActiveTexture(GL_TEXTURE0); // Reset active texture unit
    }

} // namespace GLRenderer
