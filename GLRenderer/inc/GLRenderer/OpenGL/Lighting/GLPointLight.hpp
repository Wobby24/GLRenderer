#pragma once

#include <GLRenderer/Interface/Lighting/ILightSource.hpp>
#include <glm/glm.hpp>

namespace GLRenderer {
    class GLPointLight : public ILightSource {
    public:
        GLPointLight();
        GLPointLight(const glm::vec3 position, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular);

        void applyProperties(IShader& shader) override;
        void setPosition(const glm::vec3& position);
        void setAmbient(const glm::vec3& ambient);
        void setDiffuse(const glm::vec3& diffuse);
        void setSpecular(const glm::vec3 specular);
        void setProperties(const glm::vec3& position, const glm::vec3& ambient,
            const glm::vec3& diffuse, const glm::vec3 specular);

        glm::vec3& getAmbient() { return ambient_; }
        glm::vec3& getDiffuse() { return diffuse_; }
        glm::vec3& getSpecular() { return specular_; }
        glm::vec3& getPosition() { return position_; }
        const glm::vec3& getAmbient() const { return ambient_; }
        const glm::vec3& getDiffuse() const { return diffuse_; }
        const glm::vec3& getSpecular() const { return specular_; }
        const glm::vec3& getPosition() const { return position_; }

    private:
        glm::vec3 position_ = glm::vec3(0.0);
        glm::vec3 ambient_ = glm::vec3(0.0);
        glm::vec3 diffuse_ = glm::vec3(0.0);
        glm::vec3 specular_ = glm::vec3(0.0);
        float constant_ = 0.0f;
        float linear_ = 0.0f;
        float quadratic_ = 0.0f;
        float intensity = 0.0f;
    };
}

