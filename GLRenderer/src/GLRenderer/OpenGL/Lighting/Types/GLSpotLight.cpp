#include <GLRenderer/OpenGL/Lighting/Types/GLSpotLight.hpp>
#include <GLRenderer/OpenGL/GLShader.hpp>

namespace GLRenderer {
    GLSpotLight::GLSpotLight()
        : direction_(glm::vec3(0.0f, -1.0f, 0.0f)),
        position_(glm::vec3(0.0f)),
        ambient_(glm::vec3(1.0f)),
        diffuse_(glm::vec3(1.0f)),
        specular_(glm::vec3(1.0f)),
        constant_(1.0f),
        linear_(0.09f),
        quadratic_(0.032f),
        cutOff_(glm::cos(glm::radians(12.5f))),           // <-- fixed line
        outerCutOff_(glm::cos(glm::radians(15.0f))),      // <-- separated correctly
        intensity_(1.0f),
        id_(0)
    {
    }

    GLSpotLight::GLSpotLight(const glm::vec3& direction,
        const glm::vec3& position,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float constant,
        float linear,
        float quadratic,
        float cutOff,
        float outerCutOff,
        float intensity)
        : direction_(direction),
        position_(position),
        ambient_(ambient),
        diffuse_(diffuse),
        specular_(specular),
        constant_(constant),
        linear_(linear),
        quadratic_(quadratic),
        cutOff_(cutOff),
        outerCutOff_(outerCutOff),
        intensity_(intensity),
        id_(0)
    {
    }

    void GLSpotLight::applyProperties(IShader& shader, int index) {
        GLShader* glShader = dynamic_cast<GLShader*>(&shader);
        if (!glShader) {
            std::cerr << "Shader is not a GLShader!" << std::endl;
            return;
        }

        glShader->use(); // You must bind the program before setting uniforms

        // Now it's safe to set uniforms
        glShader->setVec3("spotLights[" + std::to_string(index) + "].direction", direction_);
        glShader->setVec3("spotLights[" + std::to_string(index) + "].position", position_);
        glShader->setVec3("spotLights[" + std::to_string(index) + "].ambient", ambient_);
        glShader->setVec3("spotLights[" + std::to_string(index) + "].diffuse", diffuse_);
        glShader->setVec3("spotLights[" + std::to_string(index) + "].specular", specular_);
        glShader->setFloat("spotLights[" + std::to_string(index) + "].constant", constant_);
        glShader->setFloat("spotLights[" + std::to_string(index) + "].linear", linear_);
        glShader->setFloat("spotLights[" + std::to_string(index) + "].quadratic", quadratic_);
        glShader->setFloat("spotLights[" + std::to_string(index) + "].cutOff", cutOff_);
        glShader->setFloat("spotLights[" + std::to_string(index) + "].outerCutOff", outerCutOff_);
        glShader->setFloat("spotLights[" + std::to_string(index) + "].intensity", intensity_);
    }

    void GLSpotLight::setProperties(const glm::vec3& direction,
        const glm::vec3& position,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float constant,
        float linear,
        float quadratic,
        float cutOff,
        float outerCutOff,
        float intensity)
    {
        direction_ = direction;
        position_ = position;
        ambient_ = ambient;
        diffuse_ = diffuse;
        specular_ = specular;
        constant_ = constant;
        linear_ = linear;
        quadratic_ = quadratic;
        cutOff_ = cutOff;
        outerCutOff_ = outerCutOff;
        intensity_ = intensity;
    }
}
