#pragma once 

#include <GLRenderer/Interface/IShader.hpp>
#include <unordered_map>

namespace GLRenderer {
	class GLShader : public IShader {
	public:
		// constructor reads and builds the shader
		GLShader(std::string vertexPath, std::string fragmentPath);
		~GLShader() override;

		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;

		GLShader(GLShader&&) noexcept;
		GLShader& operator=(GLShader&&) noexcept;

		void use() const override;
		void setBool(const std::string &name, bool value) const override;
		void setInt(const std::string &name, int value) const override;
		void setFloat(const std::string &name, float value) const override;
		void setVec2(const std::string &name, const glm::vec2 &value) const override;
		void setVec2(const std::string &name, float x, float y) const override;
		void setVec3(const std::string &name, const glm::vec3 &value) const override;
		void setVec3(const std::string &name, float x, float y, float z) const override;
		void setVec4(const std::string &name, const glm::vec4 &value) const override;
		void setVec4(const std::string &name, float x, float y, float z, float w) const override;
		void setMat2(const std::string &name, const glm::mat2 &mat) const override;
		void setMat3(const std::string &name, const glm::mat3 &mat) const override;
		void setMat4(const std::string &name, const glm::mat4 &mat) const override;
		void reload(); 
		void init();
		int getUniformLocation(const std::string& name) const;
		unsigned int getID() const;
	private:
		bool isCleanedUp_ = false;
		bool isInit_ = false;
		unsigned int id_ = 0;
		std::string vertexPath_, fragmentPath_;
		mutable std::unordered_map<std::string, int> uniformLocationCache_;
		void assertInitialized() const;
		bool isGLReady() const;
		void checkCompileErrors(unsigned int shader, std::string type);
		void createShaders();
		void handleFileIO(const std::string& vertexPath, const std::string& fragmentPath,
			std::string& vertexCode, std::string& fragmentCode);
		void cleanup();
	};
}
