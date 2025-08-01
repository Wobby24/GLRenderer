#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace GLRenderer {
	class IShader
	{
	public:
		virtual ~IShader() = default;

		//use the program
		virtual void use() const = 0;
		//set methods
		virtual void setBool(const std::string& name, bool value) const = 0;
		virtual void setInt(const std::string& name, int value) const = 0;
		virtual void setFloat(const std::string& name, float value) const = 0;
	};
}
