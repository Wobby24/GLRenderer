#pragma once

#include <GLRenderer/Interface/Lighting/IMaterial.hpp>
#include <glm/glm.hpp>

namespace GLRenderer {
    class GLMaterial : public IMaterial {
    public:
        GLMaterial();
        GLMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
        //destructor wont be default once we add textures. also idk how we will use textures since GLTexture2D takes a path for its parameter idk. i might mod it to allow an overlaod for consturctor and the loadtexture functoin
        ~GLMaterial() override = default;

        void applyProperties(IShader& shader) override;

        // Setters
        void setAmbient(const glm::vec3& ambient);
        void setDiffuse(const glm::vec3& diffuse);
        void setSpecular(const glm::vec3& specular);
        void setShininess(float shininess);

        glm::vec3& getAmbient() { return ambient_; }
        glm::vec3& getDiffuse() { return diffuse_; }
        glm::vec3& getSpecular() { return specular_; }
        float& getShininess() { return shininess_; }
        const glm::vec3& getAmbient() const { return ambient_; }
        const glm::vec3& getDiffuse() const { return diffuse_; }
        const glm::vec3& getSpecular() const { return specular_; }
        const float& getShininess() const  { return shininess_; }

        void setProperties(const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular,
            float shininess);

    private:
        glm::vec3 ambient_ = glm::vec3(0.0f);
        glm::vec3 diffuse_ = glm::vec3(0.0f);
        glm::vec3 specular_ = glm::vec3(0.0f);
        float shininess_ = 32.0f;
    };
}

