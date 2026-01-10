#include <Renderer/OpenGL/Lighting/Types/GLDirLight.hpp>
#include <Renderer/OpenGL/GLShader.hpp>

namespace Renderer {
    namespace OpenGL {
        GLDirLight::GLDirLight()
            : direction_(glm::vec3(0.0f, -1.0f, 0.0f)),
            ambient_(glm::vec3(1.0f)),
            diffuse_(glm::vec3(1.0f)),
            specular_(glm::vec3(1.0f)),
            intensity_(1.0f),
            id_(0)
        {
        }

        GLDirLight::GLDirLight(const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular,
            float intensity)
            : direction_(direction),
            ambient_(ambient),
            diffuse_(diffuse),
            specular_(specular),
            intensity_(intensity),
            id_(0)
        {
        }

        void GLDirLight::applyProperties(IShader& shader, int index) {
            GLShader* glShader = dynamic_cast<GLShader*>(&shader);
            if (!glShader) {
                std::cerr << "Shader is not a GLShader!" << std::endl;
                return;
            }

            glShader->use(); // You must bind the program before setting uniforms

            // Now it's safe to set uniforms
            glShader->setVec3("dirLights[" + std::to_string(index) + "].direction", direction_);
            glShader->setVec3("dirLights[" + std::to_string(index) + "].ambient", ambient_);
            glShader->setVec3("dirLights[" + std::to_string(index) + "].diffuse", diffuse_);
            glShader->setVec3("dirLights[" + std::to_string(index) + "].specular", specular_);
            glShader->setFloat("dirLights[" + std::to_string(index) + "].intensity", intensity_);
        }

        void GLDirLight::setProperties(const glm::vec3& direction,
            const glm::vec3& ambient,
            const glm::vec3& diffuse,
            const glm::vec3& specular,
            float intensity)
        {
            direction_ = direction;
            ambient_ = ambient;
            diffuse_ = diffuse;
            specular_ = specular;
            intensity_ = intensity;
        }
    }
}
