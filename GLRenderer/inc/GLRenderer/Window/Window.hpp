#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include <GLRenderer/Window/Interface/IWindow.hpp>

namespace GLRenderer::Window
{
	class Window : public IWindow
	{
	public:
		Window(int width, int height, const std::string& title,
			int glMajorVersion = 3, int glMinorVersion = 3);
		~Window() override;

		void PollEvents() override;
		void SwapBuffers() override;
		bool ShouldClose() const override;
		void SetTitle(const std::string& title) override;
		void SetSize(int width, int height) override;
		void GetSize(int& width, int& height) const override;

		// Return native handle for internal use or advanced scenarios
		void* GetNativeHandle() const override { return static_cast<void*>(window_); }
		void processInput();

	private:
		GLFWwindow* window_ = nullptr;
		int width_ = 800;
		int height_ = 600;
		bool shouldClose_ = false;
		bool initialized_ = false;
		bool isCleanedUp_ = false;
		std::string title_ = "GLRenderer";
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	};
}
