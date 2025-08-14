#pragma once

namespace GLRenderer {
	class IShader;

	class ILightSource {
	public:
		virtual ~ILightSource() = 0;
		virtual void applyProperties(IShader& shader) = 0;
	};

	inline ILightSource::~ILightSource() = default;
}