#include <GLRenderer/OpenGL/Lighting/Types/GLPointLight.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>

namespace GLRenderer {
	GLPointLight::GLPointLight() : 
        position_(glm::vec3(0.0)), 
        ambient_(glm::vec3(1.0)), 
        diffuse_(glm::vec3(1.0)), 
        specular_(glm::vec3(1.0)), 
        constant_(1.0f), 
        linear_(0.09f), 
        quadratic_(0.032f), 
        intensity_(1.0f), 
        id_(0) 
    {}

	GLPointLight::GLPointLight(const glm::vec3& position,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float constant,
        float linear,
        float quadratic,
        float intensity)
		: position_(position), 
        ambient_(ambient), 
        diffuse_(diffuse), 
        specular_(specular), 
        constant_(constant), 
        linear_(linear), 
        quadratic_(quadratic), 
        intensity_(intensity),
        id_(0)
	{}

	void GLPointLight::applyProperties(IShader& shader, int index) {
        GLShader* glShader = dynamic_cast<GLShader*>(&shader);
        if (!glShader) {
            std::cerr << "Shader is not a GLShader!" << std::endl;
            return;
        }

        glShader->use(); // You must bind the program before setting uniforms

        // Now it's safe to set uniforms
        glShader->setVec3("pointLights[" + std::to_string(index) + "].position", position_);
        glShader->setVec3("pointLights[" + std::to_string(index) + "].ambient", ambient_);
        glShader->setVec3("pointLights[" + std::to_string(index) + "].diffuse", diffuse_);
        glShader->setVec3("pointLights[" + std::to_string(index) + "].specular", specular_);
        glShader->setFloat("pointLights[" + std::to_string(index) + "].constant", constant_);
        glShader->setFloat("pointLights[" + std::to_string(index) + "].linear", linear_);
        glShader->setFloat("pointLights[" + std::to_string(index) + "].quadratic", quadratic_);
        glShader->setFloat("pointLights[" + std::to_string(index) + "].intensity", intensity_);
	}


    void GLPointLight::setProperties(const glm::vec3& position,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float constant,
        float linear,
        float quadratic,
        float intensity)
    {
        position_ = position;
        ambient_ = ambient;
        diffuse_ = diffuse;
        specular_ = specular;
        constant_ = constant;
        linear_ = linear;
        quadratic_ = quadratic;
        intensity_ = intensity;
    }
}
