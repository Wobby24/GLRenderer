#pragma once

#include <GLRenderer/Interface/Lighting/ILightSource.hpp>
#include <glm/glm.hpp>

namespace GLRenderer {

    class GLPointLight : public ILightSource {
    public:
        // Constructors
        GLPointLight();

        GLPointLight(
            const glm::vec3& position,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular,
            float constant,
            float linear,
            float quadratic,
            float intensity
        );

        // Override from ILightSource
        LightType getType() const override { return LightType::Point; }
        int getID() const { return id_; }
        void setID(int id) override { id_ = id; }
        void applyProperties(IShader& shader, int index) override;

        // Setters
        void setPosition(const glm::vec3& position) { position_ = position; }
        void setAmbient(const glm::vec3& ambient) { ambient_ = ambient; }
        void setDiffuse(const glm::vec3& diffuse) { diffuse_ = diffuse; }
        void setSpecular(const glm::vec3& specular) { specular_ = specular; }
        void setConstant(float constant) { constant_ = constant; }
        void setLinear(float linear) { linear_ = linear; }
        void setQuadratic(float quadratic) { quadratic_ = quadratic; }
        void setIntensity(float intensity) { intensity_ = intensity; }

        void setProperties(
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& ambient = glm::vec3(1.0f),
            const glm::vec3& diffuse = glm::vec3(1.0f),
            const glm::vec3& specular = glm::vec3(1.0f),
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f,
            float intensity = 1.0f
        );

        // Getters (non-const)
        glm::vec3& getPosition() { return position_; }
        glm::vec3& getAmbient() { return ambient_; }
        glm::vec3& getDiffuse() { return diffuse_; }
        glm::vec3& getSpecular() { return specular_; }
        float& getConstant() { return constant_; }
        float& getLinear() { return linear_; }
        float& getQuadratic() { return quadratic_; }
        float& getIntensity() { return intensity_; }

        // Getters (const)
        const glm::vec3& getPosition() const { return position_; }
        const glm::vec3& getAmbient() const { return ambient_; }
        const glm::vec3& getDiffuse() const { return diffuse_; }
        const glm::vec3& getSpecular() const { return specular_; }
        float getConstant() const { return constant_; }
        float getLinear() const { return linear_; }
        float getQuadratic() const { return quadratic_; }
        float getIntensity() const { return intensity_; }

    private:
        glm::vec3 position_ = glm::vec3(0.0f);
        glm::vec3 ambient_ = glm::vec3(1.0f);
        glm::vec3 diffuse_ = glm::vec3(1.0f);
        glm::vec3 specular_ = glm::vec3(1.0f);
        float constant_ = 1.0f;
        float linear_ = 0.09f;
        float quadratic_ = 0.032f;
        float intensity_ = 1.0f;
        //id 
        int id_ = -1; // Default to invalid
    };

}
