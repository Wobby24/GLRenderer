#pragma once

#include <string>

namespace GLRenderer::Window
{
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
		virtual void* GetNativeHandle() const = 0;
	};
}
