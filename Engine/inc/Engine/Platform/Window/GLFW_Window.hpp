#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include <Platform/Window/Interface/IWindow.hpp>
#include <memory>
#include <Platform/RenderSurface/GLFW_RenderSurface.hpp>

namespace Platform
{
	namespace Window {
		// Configuration for traits such as full-screen, VSync, and other stuff
		struct WindowState {
			bool isFullscreen = false;
			bool isCursorLocked = false;
			bool VSyncEnabled = true;

			int windowedWidth = 1280;
			int windowedHeight = 720;
			int windowedPosX = 100;
			int windowedPosY = 100;
		};

		class GLFW_Window final: public IWindow
		{
		public:
			GLFW_Window();
			GLFW_Window(int width, int height, const std::string& title,
				int glMajorVersion = 3, int glMinorVersion = 3);
			~GLFW_Window() override;

			void PollEvents() override;
			void SwapBuffers() override;
			bool ShouldClose() const override;
			void SetTitle(const std::string& title) override;
			void SetSize(int width, int height) override;
			void GetSize(int& width, int& height) const override;
			glm::ivec2 GetSize() const override;
			bool IsFullscreen() const;
			bool IsVSyncEnabled() const;
			bool IsPointerLocked() const;
			void SetPosition(int x, int y);
			void GetPosition(int& x, int& y);
			void ToggleVSync() override;
			void ToggleFullscreen() override;
			void TogglePointerLock() override;
			void SetWindowIcon(const std::string& iconFilePath) override;
			glm::ivec2 GetPosition() const;
			double GetTime() const override;

			// Return native handle for internal use or advanced scenarios
			void* GetNativeHandle() const override { return static_cast<void*>(window_); }

			Platform::RenderSurface::IRenderSurface& GetSurface() const override { return *renderSurface_; }

		private:
			GLFWwindow* window_ = nullptr;
			int width_ = 800;
			int height_ = 600;
			std::string title_;
			bool shouldClose_ = false;
			bool initialized_ = false;
			bool isCleanedUp_ = false;

			WindowState state_;
			std::unique_ptr<Platform::RenderSurface::GLFW_RenderSurface> renderSurface_;

			static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
			void HandleInternalInput();
		};
	}
}
