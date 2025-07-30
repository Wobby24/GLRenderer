#pragma once 

#include <GLRenderer/Interface/IShader.hpp>

class GLShader : public IShader{
public:
	// constructor reads and builds the shader
	GLShader(const char* vertexPath, const char* fragmentPath);
	~GLShader() = default override;

	GLShader(const GLShader&) = delete;
	GLShader& operator=(const GLShader&) = delete;

	void use() const override;
	void setBool(const std::string& name, bool value) const override;
	void setInt(const std::string& name, int value) const override;
	void setFloat(const std::string& name, float value) const override;
	unsigned int getID() const override;
private:
	unsigned int ID;
};