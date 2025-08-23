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

        // Bind Diffuse Textures
        if (!diffuseMaps_.empty()) {
            glShader->setInt("numDiffuseTextures", static_cast<int>(diffuseMaps_.size()));
            for (size_t i = 0; i < diffuseMaps_.size(); ++i) {
                int texUnit = static_cast<int>(i);
                glShader->setInt(("material.diffuse[" + std::to_string(i) + "]").c_str(), texUnit);
                diffuseMaps_[i]->bind(texUnit);
            }
            glShader->setBool("useDiffuseMap", true);
        }
        else {
            glShader->setInt("numDiffuseTextures", 0);
            glShader->setBool("useDiffuseMap", false);
        }

        // Bind Specular Textures
        if (!specularMaps_.empty()) {
            glShader->setInt("numSpecularTextures", static_cast<int>(specularMaps_.size()));
            for (size_t i = 0; i < specularMaps_.size(); ++i) {
                int texUnit = static_cast<int>(diffuseMaps_.size() + i); // Avoid overlapping units
                glShader->setInt(("material.specular[" + std::to_string(i) + "]").c_str(), texUnit);
                specularMaps_[i]->bind(texUnit);
            }
            glShader->setBool("useSpecularMap", true);
        }
        else {
            glShader->setInt("numSpecularTextures", 0);
            glShader->setBool("useSpecularMap", false);
        }

        // Bind Emissive Textures
        if (!emissionMaps_.empty()) {
            glShader->setInt("numEmissionTextures", static_cast<int>(emissionMaps_.size()));
            for (size_t i = 0; i < emissionMaps_.size(); ++i) {
                int texUnit = static_cast<int>(diffuseMaps_.size() + specularMaps_.size() + i);
                glShader->setInt(("material.emission[" + std::to_string(i) + "]").c_str(), texUnit);
                emissionMaps_[i]->bind(texUnit);
            }
            glShader->setFloat("material.emissionIntensity", emissionIntensity_);
            glShader->setBool("useEmissionMap", true);
        }
        else {
            glShader->setInt("numEmissionTextures", 0);
            glShader->setFloat("material.emissionIntensity", 0.0f);
            glShader->setBool("useEmissionMap", false);
        }

        // Set shininess regardless
        glShader->setFloat("material.shininess", shininess_);
    }

    void GLMaterial::addDiffuseTexture(std::shared_ptr<GLTexture2D> texture) {
        if (diffuseMaps_.size() >= MAX_TEXTURES_PER_TYPE) {
            throw std::runtime_error("GLMaterial::addDiffuseTexture: Max texture limit for diffuse has already been reached! Cannot add new texture!");
        }
        else
            diffuseMaps_.push_back(std::move(texture));
    }

    void GLMaterial::addSpecularTexture(std::shared_ptr<GLTexture2D> texture) {
        if (specularMaps_.size() >= MAX_TEXTURES_PER_TYPE) {
            throw std::runtime_error("GLMaterial::addSpecularTexture: Max texture limit for specular has already been reached! Cannot add new texture!");
        }
        else
            specularMaps_.push_back(std::move(texture));
    }

    void GLMaterial::addEmissionTexture(std::shared_ptr<GLTexture2D> texture) {
        if (emissionMaps_.size() >= MAX_TEXTURES_PER_TYPE) {
            throw std::runtime_error("GLMaterial::addEmissiveTexture: Max texture limit for emission has already been reached! Cannot add new texture!");
        }
        else
            emissionMaps_.push_back(std::move(texture));
    }

    void GLMaterial::setDiffuseTextures(const std::vector<std::shared_ptr<GLTexture2D>>& textures) {
        diffuseMaps_ = textures;
    }

    void GLMaterial::setSpecularTextures(const std::vector<std::shared_ptr<GLTexture2D>>& textures) {
        specularMaps_ = textures;
    }

    void GLMaterial::setEmissionTextures(const std::vector<std::shared_ptr<GLTexture2D>>& textures) {
        emissionMaps_ = textures;
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

    std::vector<std::shared_ptr<GLTexture2D>>& GLMaterial::getDiffuseTextures() {
        return diffuseMaps_;
    }

    const std::vector<std::shared_ptr<GLTexture2D>>& GLMaterial::getDiffuseTextures() const {
        return diffuseMaps_;
    }

    std::vector<std::shared_ptr<GLTexture2D>>& GLMaterial::getSpecularTextures() {
        return specularMaps_;
    }

    const std::vector<std::shared_ptr<GLTexture2D>>& GLMaterial::getSpecularTextures() const {
        return specularMaps_;
    }

    std::vector<std::shared_ptr<GLTexture2D>>& GLMaterial::getEmissionTextures() {
        return emissionMaps_;
    }

    const std::vector<std::shared_ptr<GLTexture2D>>& GLMaterial::getEmissionTextures() const {
        return emissionMaps_;
    }

}
