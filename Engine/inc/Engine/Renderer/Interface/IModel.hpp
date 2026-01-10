#pragma once

namespace Renderer {
	class IShader;

	class IModel {
	public:
		virtual ~IModel() = 0;

		virtual void Draw(IShader& shader) = 0;
	};

	inline IModel::~IModel() {}  // Provide empty inline definition in the header
}