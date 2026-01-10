#pragma once

#include <Renderer/Interface/Lighting/ILightSource.hpp>
#include <glm/glm.hpp>

namespace Renderer {
	namespace OpenGL {
		class GLSpotLight : public ILightSource {
		public:
			// Constructors
			GLSpotLight();
			GLSpotLight(
				const glm::vec3& direction,
				const glm::vec3& position,
				const glm::vec3& ambient,
				const glm::vec3& diffuse,
				const glm::vec3& specular,
				float constant,
				float linear,
				float quadratic,
				float cutOff,
				float outerCutOff,
				float intensity
			);

			// Light type override
			LightType getType() const override { return LightType::Spot; }
			int getID() const { return id_; }
			void setID(int id) override { id_ = id; }
			// Applies properties to a shader (you may want to pass an index if handling arrays)
			void applyProperties(IShader& shader, int index) override;

			// Setters
			void setPosition(const glm::vec3& position) { position_ = position; }
			void setDirection(const glm::vec3& direction) { direction_ = direction; }
			void setAmbient(const glm::vec3& ambient) { ambient_ = ambient; }
			void setDiffuse(const glm::vec3& diffuse) { diffuse_ = diffuse; }
			void setSpecular(const glm::vec3& specular) { specular_ = specular; }
			void setConstant(float constant) { constant_ = constant; }
			void setLinear(float linear) { linear_ = linear; }
			void setQuadratic(float quadratic) { quadratic_ = quadratic; }
			void setCutOff(float cutOff) { cutOff_ = cutOff; }
			void setOuterCutOff(float outerCutOff) { outerCutOff_ = outerCutOff; }
			void setIntensity(float intensity) { intensity_ = intensity; }

			// Set all properties at once
			void setProperties(
				const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
				const glm::vec3& position = glm::vec3(0.0f),
				const glm::vec3& ambient = glm::vec3(1.0f),
				const glm::vec3& diffuse = glm::vec3(1.0f),
				const glm::vec3& specular = glm::vec3(1.0f),
				float constant = 1.0f,
				float linear = 0.09f,
				float quadratic = 0.032f,
				float cutOff = glm::cos(glm::radians(12.5f)),
				float outerCutOff = glm::cos(glm::radians(15.0f)),
				float intensity = 1.0f
			);

			// Getters (non-const refs, allow modification)
			glm::vec3& getDirection() { return direction_; }
			glm::vec3& getPosition() { return position_; }
			glm::vec3& getAmbient() { return ambient_; }
			glm::vec3& getDiffuse() { return diffuse_; }
			glm::vec3& getSpecular() { return specular_; }
			float& getConstant() { return constant_; }
			float& getLinear() { return linear_; }
			float& getQuadratic() { return quadratic_; }
			float& getCutOff() { return cutOff_; }
			float& getOuterCutOff() { return outerCutOff_; }
			float& getIntensity() { return intensity_; }

			// Const getters
			const glm::vec3& getDirection() const { return direction_; }
			const glm::vec3& getPosition() const { return position_; }
			const glm::vec3& getAmbient() const { return ambient_; }
			const glm::vec3& getDiffuse() const { return diffuse_; }
			const glm::vec3& getSpecular() const { return specular_; }
			const float& getConstant() const { return constant_; }
			const float& getLinear() const { return linear_; }
			const float& getQuadratic() const { return quadratic_; }
			const float& getCutOff() const { return cutOff_; }
			const float& getOuterCutOff() const { return outerCutOff_; }
			const float& getIntensity() const { return intensity_; }

		private:
			glm::vec3 direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
			glm::vec3 position_ = glm::vec3(0.0f);
			glm::vec3 ambient_ = glm::vec3(1.0f);
			glm::vec3 diffuse_ = glm::vec3(1.0f);
			glm::vec3 specular_ = glm::vec3(1.0f);
			float constant_ = 1.0f;
			float linear_ = 0.09f;
			float quadratic_ = 0.032f;
			float cutOff_ = glm::cos(glm::radians(12.5f));
			float outerCutOff_ = glm::cos(glm::radians(15.0f));
			float intensity_ = 1.0f;
			//id
			int id_ = -1; // Default to invalid
		};
	}
}
