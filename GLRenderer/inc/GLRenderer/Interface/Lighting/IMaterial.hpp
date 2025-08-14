#pragma once

namespace GLRenderer {
	// Forward declaration of IShader
	class IShader;

	class IMaterial {
	public:
		virtual ~IMaterial() = 0;
		virtual void applyProperties(IShader& shader) = 0;
	};

	// nline destructor definition to avoid multiple definition errors
	inline IMaterial::~IMaterial() = default;
}
