#pragma once

#include <string>
#include <glm/glm.hpp>

namespace GLRenderer {

	class IRenderSurface; // forward declaration

	namespace Window {

		class IWindow
		{
		public:
			virtual ~IWindow() = default;

			virtual void PollEvents() = 0;
			virtual void SwapBuffers() = 0;
			virtual bool ShouldClose() const = 0;
			virtual void SetTitle(const std::string& title) = 0;
			virtual void SetSize(int width, int height) = 0;
			virtual void GetSize(int& width, int& height) const = 0;
			virtual glm::ivec2 GetSize() const = 0;
			virtual void* GetNativeHandle() const = 0;
			// Access the render surface associated with this window
			virtual ::GLRenderer::IRenderSurface* GetSurface() = 0; // non-owning
		};
	}
}
