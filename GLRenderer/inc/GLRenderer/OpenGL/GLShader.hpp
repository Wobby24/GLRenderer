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
		void setBool(const std::string& name, bool value) const override;
		void setInt(const std::string& name, int value) const override;
		void setFloat(const std::string& name, float value) const override;
		void reload(); 
		void init();
		int getUniformLocation(const std::string& name) const;
		unsigned int getID() const;
	private:
		bool isCleanedUp_ = false;
		unsigned int id_ = 0;
		mutable std::unordered_map<std::string, int> uniformLocationCache_;
		std::string vertexPath_, fragmentPath_;
		bool isGLReady() const;
		void checkCompileErrors(unsigned int shader, std::string type);
		void createShaders();
		void handleFileIO(std::ifstream& vShaderFile, std::ifstream& fShaderFile, std::string& vertexCode, std::string& fragmentCode);
		void cleanup();
	};
}
