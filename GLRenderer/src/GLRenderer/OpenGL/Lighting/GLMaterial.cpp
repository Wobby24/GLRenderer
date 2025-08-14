#include <GLRenderer/OpenGL/Lighting/GLMaterial.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>

namespace GLRenderer {

	GLMaterial::GLMaterial() : ambient_(glm::vec3(0.0f)), diffuse_(glm::vec3(0.0f)), specular_(glm::vec3(0.0f)), shininess_(0.0f) {}
	GLMaterial::GLMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : ambient_(ambient), diffuse_(diffuse), specular_(specular), shininess_(shininess) {}

	void GLMaterial::applyProperties(IShader& shader) {
        GLShader* glShader = dynamic_cast<GLShader*>(&shader);
        if (!glShader) {
            std::cerr << "Shader is not a GLShader!" << std::endl;
            return;
        }

        glShader->use(); // You must bind the program before setting uniforms

        // Now it's safe to set uniforms
        glShader->setVec3("material.ambient", ambient_);
        glShader->setVec3("material.diffuse", diffuse_);
        glShader->setVec3("material.specular", specular_);
        glShader->setFloat("material.shininess", shininess_);
	}

    void GLMaterial::setAmbient(const glm::vec3& ambient) {
        ambient_ = ambient;
    }

    void GLMaterial::setDiffuse(const glm::vec3& diffuse) {
        diffuse_ = diffuse;
    }

    void GLMaterial::setSpecular(const glm::vec3& specular) {
        specular_ = specular;
    }

    void GLMaterial::setShininess(float shininess) {
        shininess_ = shininess;
    }

    void GLMaterial::setProperties(const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float shininess) 
    {
        ambient_ = ambient;
        diffuse_ = diffuse;
        specular_ = specular;
        shininess_ = shininess;
    }
}