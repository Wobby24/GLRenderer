// GLRendererContextDesc.hpp
#pragma once
#include <Renderer/Interface/Types/IRendererContextDesc.hpp>

namespace Renderer
{
	namespace OpenGL {
		struct GLRendererContextDesc : public Renderer::IRendererContextDesc {
			int width = 800;
			int height = 600;
			int majorVersion = 3;
			int minorVersion = 3;
		};
	}
}
