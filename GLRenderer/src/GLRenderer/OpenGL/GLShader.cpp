#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif


#include "GLRenderer/OpenGL/GLShader.hpp"
#include <filesystem>
#include <string>

namespace GLRenderer {

	GLShader::GLShader(std::string vertexPath, std::string fragmentPath)
		:  isCleanedUp_(false), isInit_(0), id_(0), vertexPath_(std::move(vertexPath)), fragmentPath_(std::move(fragmentPath))
	{}

	GLShader::~GLShader() {
		cleanup();
	}

	void GLShader::cleanup() {
		if (isCleanedUp_) return;

		if (id_ != 0) {
			glDeleteProgram(id_);
			id_ = 0;
		}
		isCleanedUp_ = true;
	}

	std::filesystem::path getExecutableDir() {
#ifdef _WIN32
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		return std::filesystem::path(buffer).parent_path();

#elif __APPLE__
		char buffer[1024];
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0) {
			return std::filesystem::path(buffer).parent_path();
		}
		return {}; // Failed

#elif __linux__
		char buffer[1024];
		ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
		if (count != -1) {
			return std::filesystem::path(std::string(buffer, count)).parent_path();
		}
		return {}; // Failed

#else
		return {}; // Unsupported platform
#endif
	}

	std::string GLShader::getAbsoluteShaderPath(const std::string& path) {
		// Step 1: Get the directory of the current executable
		std::filesystem::path exeDir = getExecutableDir();

		// Step 2: Define the path to the shader source folder (relative to executable)
		std::filesystem::path sourceShadersRoot = exeDir / "../../../../../GLRenderer/GLRenderer/res/Shaders";
		sourceShadersRoot = sourceShadersRoot.lexically_normal();

		// Step 3: Take the path passed in (e.g., "res/Shaders/Scenes/C2/lighting.vert")
		// and remove "res/Shaders/" from the start so we get "Scenes/C2/lighting.vert"
		std::filesystem::path inputPath = path;
		std::filesystem::path baseRel = "res/Shaders";

		std::filesystem::path relativePath;
		if (inputPath.string().find(baseRel.string()) != std::string::npos) {
			relativePath = std::filesystem::relative(inputPath, baseRel);
		}
		else {
			relativePath = inputPath;  // fallback if base doesn't match
		}

		// Step 4: Construct the final path
		std::filesystem::path fullShaderPath = sourceShadersRoot / relativePath;
		fullShaderPath = fullShaderPath.lexically_normal();

		return fullShaderPath.string();
	}


	//use program method
	void GLShader::use() const { 
		assertInitialized();
		glUseProgram(id_); 
	}

	//get method for the ID
	unsigned int GLShader::getID() const { return id_; }

	/* Set uniform methods */
	//bool
	void GLShader::setBool(const std::string& name, bool value) const {
		assertInitialized();
		glUniform1i(getUniformLocation(name), (int)value);
	}

	//int
	void GLShader::setInt(const std::string& name, int value) const {
		assertInitialized();
		glUniform1i(getUniformLocation(name), value);
	}

	//float
	void GLShader::setFloat(const std::string& name, float value) const {
		assertInitialized();
		glUniform1f(getUniformLocation(name), value);
	}

	void GLShader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		assertInitialized();
		glUniform2fv(getUniformLocation(name), 1, &value[0]);
	}

	void GLShader::setVec2(const std::string& name, float x, float y) const
	{
		assertInitialized();
		glUniform2f(getUniformLocation(name), x, y);
	}

	// ------------------------------------------------------------------------

	void GLShader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		assertInitialized();
		glUniform3fv(getUniformLocation(name), 1, &value[0]);
	}

	void GLShader::setVec3(const std::string& name, float x, float y, float z) const
	{
		assertInitialized();
		glUniform3f(getUniformLocation(name), x, y, z);
	}

	// ------------------------------------------------------------------------

	void GLShader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		assertInitialized();
		glUniform4fv(getUniformLocation(name), 1, &value[0]);
	}

	void GLShader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		assertInitialized();
		glUniform4f(getUniformLocation(name), x, y, z, w);
	}

	// ------------------------------------------------------------------------

	void GLShader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		assertInitialized();
		glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	// ------------------------------------------------------------------------

	void GLShader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		assertInitialized();
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	// ------------------------------------------------------------------------

	void GLShader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		assertInitialized();
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}


	//checks if gl is init
	bool GLShader::isGLReady() const {
		return glCreateShader != nullptr; // or check any known GL function
	}

	//use this to create shaders/init them when first using them
	void GLShader::init() {
		if (!isGLReady()) {
			throw std::runtime_error("Cannot initialize shader: OpenGL context not ready.");
		}
		isInit_ = true;  // mark as initialized first!
		reload();        // safe to call now
	}

	//use to check if init without needing if statements everywhere
	void GLShader::assertInitialized() const {
		if (!isInit_) {
			throw std::runtime_error("GLShader is not initialized. Call init() first.");
		}
	}

	//Create Shaders
	void GLShader::createShaders() {
		//1. retrieve the vertex/fragment source code from the file path
		std::string vertexCode_, fragmentCode_;

		//function will handle file io for teh ifstreams and then put them into our vertex code and fragment code
		std::string absVertPath = getAbsoluteShaderPath(vertexPath_);
		std::string absFragPath = getAbsoluteShaderPath(fragmentPath_);

		handleFileIO(absVertPath, absFragPath, vertexCode_, fragmentCode_);

		const char* vShaderCode = vertexCode_.c_str();
		const char* fShaderCode = fragmentCode_.c_str();

		//2. Compile and link the shaders
		unsigned int vertex, fragment;

		//vert shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		//frag shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		//shader program
		id_ = glCreateProgram();
		glAttachShader(id_, vertex);
		glAttachShader(id_, fragment);
		glLinkProgram(id_);
		checkCompileErrors(id_, "PROGRAM");
		//delete shaders as they are linked into the program and are no longer needed
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void GLShader::reload() {
		assertInitialized();

		if (!isCleanedUp_) {
			cleanup();
		}
		isCleanedUp_ = false; // mark as active again

		uniformLocationCache_.clear();
		createShaders();
	}


	int GLShader::getUniformLocation(const std::string& name) const {
		assertInitialized();
		// Check cache first
		auto it = uniformLocationCache_.find(name);
		if (it != uniformLocationCache_.end()) {
			return it->second;
		}
		// Query OpenGL for the uniform location
		int location = glGetUniformLocation(id_, name.c_str());
		if (location == -1) {
			std::cerr << "Warning: uniform '" << name << "' doesn't exist or is not used in shader.\n";
		}
		uniformLocationCache_[name] = location;
		return location;
	}

	void GLShader::handleFileIO(const std::string& vertexPath, const std::string& fragmentPath,
		std::string& vertexCode, std::string& fragmentCode) {
		try {
			std::ifstream vShaderFile(vertexPath);
			if (!vShaderFile.is_open()) {
				throw std::runtime_error("Failed to open vertex shader file: " + vertexPath);
			}

			std::ifstream fShaderFile(fragmentPath);
			if (!fShaderFile.is_open()) {
				throw std::runtime_error("Failed to open fragment shader file: " + fragmentPath);
			}

			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (const std::ifstream::failure& e) {
			throw std::runtime_error("Shader file I/O failure: " + std::string(e.what()));
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Unhandled error while reading shader files: " + std::string(e.what()));
		}
	}

	void GLShader::checkCompileErrors(unsigned int shader, std::string type)
	{
		assertInitialized();
		int success;
		char infoLog[1024];

		// Check errors not relating to the program 
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::string errorMsg = "Shader compilation failed for shader type: " + type + "\n" + infoLog;
				std::cerr << errorMsg << std::endl;
				throw std::runtime_error(errorMsg);
			}
		}
		//check programs that don't relate to shader compilation
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::string errorMsg = "Shader program linkage failed for shader type: " + type + "\n" + infoLog;
				std::cerr << errorMsg << std::endl;
				throw std::runtime_error(errorMsg);
			}
		}
	}

};