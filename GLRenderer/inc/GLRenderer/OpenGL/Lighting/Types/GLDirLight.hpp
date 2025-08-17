#pragma once

#include <GLRenderer/Interface/Lighting/ILightSource.hpp>
#include <glm/glm.hpp>

namespace GLRenderer {

    class GLDirLight : public ILightSource {
    public:

        // Constructors
        GLDirLight();

        GLDirLight(
            const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular,
            float intensity
        );

        // Override
        LightType getType() const override { return LightType::Directional; }
        int getID() const { return id_; }
        void setID(int id) override { id_ = id; }
        // Applies properties to a shader (you may want to pass an index if handling arrays)
        void applyProperties(IShader& shader, int index) override;

        // Setters
        void setDirection(const glm::vec3& direction) { direction_ = direction; }
        void setAmbient(const glm::vec3& ambient) { ambient_ = ambient; }
        void setDiffuse(const glm::vec3& diffuse) { diffuse_ = diffuse; }
        void setSpecular(const glm::vec3& specular) { specular_ = specular; }
        void setIntensity(float intensity) { intensity_ = intensity; }

        void setProperties(
            const glm::vec3& direction = glm::vec3(-0.2f, -1.0f, -0.3f),
            const glm::vec3& ambient = glm::vec3(1.0f),
            const glm::vec3& diffuse = glm::vec3(1.0f),
            const glm::vec3& specular = glm::vec3(1.0f),
            float intensity = 1.0f
        );

        // Getters
        const glm::vec3& getDirection() const { return direction_; }
        const glm::vec3& getAmbient() const { return ambient_; }
        const glm::vec3& getDiffuse() const { return diffuse_; }
        const glm::vec3& getSpecular() const { return specular_; }
        float getIntensity() const { return intensity_; }

    private:
        glm::vec3 direction_ = glm::vec3(-0.2f, -1.0f, -0.3f);
        glm::vec3 ambient_ = glm::vec3(1.0f);
        glm::vec3 diffuse_ = glm::vec3(1.0f);
        glm::vec3 specular_ = glm::vec3(1.0f);
        float intensity_ = 1.0f;
        //id for manager
        int id_ = -1; // Default to invalid
    };

}
