#include <GLRenderer/OpenGL/Lighting/GLPointLight.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>

namespace GLRenderer {
	GLPointLight::GLPointLight() : position_(glm::vec3(0.0)), ambient_(glm::vec3(0.0)), diffuse_(glm::vec3(0.0)), specular_(glm::vec3(0.0)), constant_(0.0f), linear_(0.0f), quadratic_(0.0f), intensity(0.0f) {}

	GLPointLight::GLPointLight(const glm::vec3 position, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular) 
		: position_(position), ambient_(ambient), diffuse_(diffuse), specular_(specular), constant_(0.0f), linear_(0.0f), quadratic_(0.0f), intensity(0.0f)
	{}

	void GLPointLight::applyProperties(IShader& shader) {
        GLShader* glShader = dynamic_cast<GLShader*>(&shader);
        if (!glShader) {
            std::cerr << "Shader is not a GLShader!" << std::endl;
            return;
        }

        glShader->use(); // You must bind the program before setting uniforms

        // Now it's safe to set uniforms
        glShader->setVec3("light.position", position_);
        glShader->setVec3("light.ambient", ambient_);
        glShader->setVec3("light.diffuse", diffuse_);
        glShader->setVec3("light.specular", specular_);
	}

    void GLPointLight::setPosition(const glm::vec3& position) {
        position_ = position;
    }

    void GLPointLight::setAmbient(const glm::vec3& ambient) {
        ambient_ = ambient;
    }

    void GLPointLight::setDiffuse(const glm::vec3& diffuse) {
        diffuse_ = diffuse;
    }

    void GLPointLight::setSpecular(const glm::vec3 specular) {
        specular_ = specular;
    }

    void GLPointLight::setProperties(const glm::vec3& position, const glm::vec3& ambient,
        const glm::vec3& diffuse, const glm::vec3 specular) {
        position_ = position;
        ambient_ = ambient;
        diffuse_ = diffuse;
        specular_ = specular;
    }
}
