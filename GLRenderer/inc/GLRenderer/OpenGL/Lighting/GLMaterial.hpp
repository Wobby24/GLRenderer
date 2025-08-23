#pragma once

#include <GLRenderer/Interface/Lighting/IMaterial.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <vector>
#include <memory>

namespace GLRenderer {
    class GLMaterial : public IMaterial {
    public:
        GLMaterial();
        ~GLMaterial() override = default;

        static constexpr int MAX_TEXTURES_PER_TYPE = 8;

        void applyProperties(IShader& shader) override;

        // Setters
        void addDiffuseTexture(std::shared_ptr<GLTexture2D> texture);
        void addSpecularTexture(std::shared_ptr<GLTexture2D> texture);
        void addEmissionTexture(std::shared_ptr<GLTexture2D> texture);
        void setShininess(float shininess);
        void setEmissionIntensity(float intensity) { emissionIntensity_ = intensity; }

        void setDiffuseTextures(const std::vector<std::shared_ptr<GLTexture2D>>& textures);
        void setSpecularTextures(const std::vector<std::shared_ptr<GLTexture2D>>& textures);
        void setEmissionTextures(const std::vector<std::shared_ptr<GLTexture2D>>& textures);

        // Non-const getter returns shared_ptr<T>&
        std::vector<std::shared_ptr<GLTexture2D>>& getDiffuseTextures();

        // Const getter returns shared_ptr<const GLTexture2D> by value
        const std::vector<std::shared_ptr<GLTexture2D>>& getDiffuseTextures() const;

        std::vector<std::shared_ptr<GLTexture2D>>& getSpecularTextures();
        const std::vector<std::shared_ptr<GLTexture2D>>& getSpecularTextures() const;

        std::vector<std::shared_ptr<GLTexture2D>>& getEmissionTextures();
        const std::vector<std::shared_ptr<GLTexture2D>>& getEmissionTextures() const;

        // Getter/setter for shininess
        float& getShininess();
        const float& getShininess() const;
        float& getEmissionIntensity();
        const float& getEmissionIntensity() const;

    private:
        float shininess_ = 32.0f;
        float emissionIntensity_ = 1.0f;

        std::vector<std::shared_ptr<GLTexture2D>> diffuseMaps_;
        std::vector<std::shared_ptr<GLTexture2D>> specularMaps_;
        std::vector<std::shared_ptr<GLTexture2D>> emissionMaps_;

        // Add functions to get the max textures per type
        int getNumDiffuseTextures() const { return static_cast<int>(diffuseMaps_.size()); }
        int getNumSpecularTextures() const { return static_cast<int>(specularMaps_.size()); }
        int getNumEmissionTextures() const { return static_cast<int>(emissionMaps_.size()); }
    };
}
