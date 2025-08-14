#include <GLRenderer/OpenGL/Lighting/GLMaterial.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <iostream>

namespace GLRenderer {

    GLMaterial::GLMaterial() = default;

    void GLMaterial::applyProperties(IShader& shader) {
        GLShader* glShader = dynamic_cast<GLShader*>(&shader);
        if (!glShader) {
            std::cerr << "Shader is not a GLShader!" << std::endl;
            return;
        }

        glShader->use();

        // ==== Diffuse ====
        if (hasDiffuseTexture()) {
            glShader->setInt("useDiffuseMap", 1);
            glShader->setInt("material.diffuse", 0);  // Texture unit 0
            diffuseMap_->bind(0);
        } else {
            glShader->setInt("useDiffuseMap", 0);
        }

        // ==== Specular ====
        if (hasSpecularTexture()) {
            glShader->setInt("useSpecularMap", 1);
            glShader->setInt("material.specular", 1); // Texture unit 1
            specularMap_->bind(1);
        }
        else {
            glShader->setInt("useSpecularMap", 0);
        }

        // ==== Emissive ====
        if (hasEmissiveTexture()) {
            glShader->setInt("useEmissionMap", 1);
            glShader->setInt("material.emission", 2);
            emissiveMap_->bind(2);
        }
        else {
            glShader->setInt("useEmissionMap", 0);
        }

        // Set shininess regardless
        glShader->setFloat("material.shininess", shininess_);
    }


    void GLMaterial::setDiffuseTexture(std::shared_ptr<GLTexture2D> texture) {
        diffuseMap_ = texture;
    }

    void GLMaterial::setSpecularTexture(std::shared_ptr<GLTexture2D> texture) {
        specularMap_ = texture;
    }

    void GLMaterial::setEmissiveTexture(std::shared_ptr<GLTexture2D> texture) {
        emissiveMap_ = texture;
    }

    void GLMaterial::setupProperties(
        std::shared_ptr<GLTexture2D> diffuseTexture,
        std::shared_ptr<GLTexture2D> specularTexture,
        std::shared_ptr<GLTexture2D> emissiveTexture
    ) {
        setDiffuseTexture(diffuseTexture);
        setSpecularTexture(specularTexture);
        setEmissiveTexture(emissiveTexture);
    }

    bool GLMaterial::hasDiffuseTexture() const {
        return diffuseMap_ != nullptr;
    }

    bool GLMaterial::hasSpecularTexture() const {
        return specularMap_ != nullptr;
    }

    bool GLMaterial::hasEmissiveTexture() const {
        return emissiveMap_ != nullptr;
    }

    void GLMaterial::setShininess(float shininess) {
        shininess_ = shininess;
    }

    float& GLMaterial::getShininess() {
        return shininess_;
    }

    const float& GLMaterial::getShininess() const {
        return shininess_;
    }

    std::shared_ptr<GLTexture2D>& GLMaterial::getDiffuseTexture() {
        return diffuseMap_;
    }

    std::shared_ptr<const GLTexture2D> GLMaterial::getDiffuseTexture() const {
        return diffuseMap_;
    }

    std::shared_ptr<GLTexture2D>& GLMaterial::getSpecularTexture() {
        return specularMap_;
    }

    std::shared_ptr<const GLTexture2D> GLMaterial::getSpecularTexture() const {
        return specularMap_;
    }

    std::shared_ptr<GLTexture2D>& GLMaterial::getEmissiveTexture() {
        return emissiveMap_;
    }

    std::shared_ptr<const GLTexture2D> GLMaterial::getEmissiveTexture() const {
        return emissiveMap_;
    }

}
