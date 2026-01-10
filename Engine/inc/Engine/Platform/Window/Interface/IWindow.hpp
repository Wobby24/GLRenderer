#pragma once

#include <string>
#include <Platform/RenderSurface/Interface/IRenderSurface.hpp>
#include <glm/glm.hpp>

namespace Platform {
	namespace Window {
		class IRenderSurface;

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
			virtual void ToggleVSync() = 0;
			virtual void TogglePointerLock() = 0;
			virtual void SetWindowIcon(const std::string& path) = 0;
			virtual void ToggleFullscreen() = 0;
			virtual double GetTime() const = 0;
			// Access the render surface associated with this window
			virtual Platform::RenderSurface::IRenderSurface& GetSurface() const = 0; // non-owning
		};
	}
}
