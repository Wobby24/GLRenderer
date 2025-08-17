#pragma once

#include <GLRenderer/Interface/Lighting/IMaterial.hpp>
#include <GLRenderer/OpenGL/GLTexture.hpp>
#include <memory>

namespace GLRenderer {
    class GLMaterial : public IMaterial {
    public:
        GLMaterial();
        ~GLMaterial() override = default;

        void applyProperties(IShader& shader) override;

        // Setters
        void setDiffuseTexture(std::shared_ptr<GLTexture2D> texture);
        void setSpecularTexture(std::shared_ptr<GLTexture2D> texture);
        void setEmissiveTexture(std::shared_ptr<GLTexture2D> texture);
        void setShininess(float shininess);
        void setEmissionIntensity(float intensity);

        void setupProperties(
            std::shared_ptr<GLTexture2D> diffuseTexture,
            std::shared_ptr<GLTexture2D> specularTexture,
            std::shared_ptr<GLTexture2D> emissiveTexture
        );

        // Non-const getter returns shared_ptr<T>&
        std::shared_ptr<GLTexture2D>& getDiffuseTexture();

        // Const getter returns shared_ptr<const GLTexture2D> by value
        std::shared_ptr<const GLTexture2D> getDiffuseTexture() const;

        std::shared_ptr<GLTexture2D>& getSpecularTexture();
        std::shared_ptr<const GLTexture2D> getSpecularTexture() const;

        std::shared_ptr<GLTexture2D>& getEmissiveTexture();
        std::shared_ptr<const GLTexture2D> getEmissiveTexture() const;

        // Getter/setter for shininess
        float& getShininess();
        const float& getShininess() const;
        float& getEmissionIntensity();
        const float& getEmissionIntensity() const;

        bool hasDiffuseTexture() const;
        bool hasSpecularTexture() const;
        bool hasEmissiveTexture() const;

    private:
        float shininess_ = 32.0f;
        float emissionIntensity = 1.0f;

        std::shared_ptr<GLTexture2D> diffuseMap_ = nullptr;
        std::shared_ptr<GLTexture2D> specularMap_ = nullptr;
        std::shared_ptr<GLTexture2D> emissiveMap_ = nullptr;
    };
}
