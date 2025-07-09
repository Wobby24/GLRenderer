// GLRendererContextDesc.hpp
#pragma once
#include <GLRenderer/Interface/Types/IRendererContextDesc.hpp>

namespace GLRenderer
{
	struct GLRendererContextDesc : public IRendererContextDesc {
		int width = 800;
		int height = 600;
		int majorVersion = 3;
		int minorVersion = 3;
	};
}
