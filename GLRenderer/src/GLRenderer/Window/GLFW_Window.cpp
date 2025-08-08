#include <GLRenderer/Window/GLFW_Window.hpp>
#include <string>
#include <stdexcept>
#include <GLFW/glfw3.h> // Needed for GLFW calls

namespace GLRenderer::Window
{
	GLFW_Window::GLFW_Window(int width, int height, const std::string& title, int glMajorVersion, int glMinorVersion)
		: window_(nullptr), width_(width), height_(height), title_(title), shouldClose_(false), initialized_(false), isCleanedUp_(false)
	{
		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
		if (!window_) {
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}
		glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
 
		glfwMakeContextCurrent(window_);
		glfwSwapInterval(1);  // 0 = VSync off, 1 = VSync on
		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	GLFW_Window::~GLFW_Window()
	{
		if (isCleanedUp_) return;

		glfwDestroyWindow(window_);
		glfwTerminate();
		isCleanedUp_ = true;
	}

	void GLFW_Window::HandleInternalInput()
	{
		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window_, true);

		static bool f11Pressed = false;
		if (glfwGetKey(window_, GLFW_KEY_F11) == GLFW_PRESS) {
			if (!f11Pressed) {
				ToggleFullscreen();
				f11Pressed = true;
			}
		}
		else {
			f11Pressed = false;
		}

		static bool tabPressed = false;
		if (glfwGetKey(window_, GLFW_KEY_TAB) == GLFW_PRESS) {
			if (!tabPressed) {
				TogglePointerLock();
				tabPressed = true;
			}
		}
		else {
			tabPressed = false;
		}
	}

	void GLFW_Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		(void)window; // Silence unused parameter warning
		glViewport(0, 0, width, height);
	}

	void GLFW_Window::SetTitle(const std::string& title)
	{
		title_ = title;
		if (window_)
			glfwSetWindowTitle(window_, title.c_str());
	}
	
	void GLFW_Window::SetSize(int width, int height)
	{
		width_ = width;
		height_ = height;
		if (window_)
			glfwSetWindowSize(window_, width_, height_);
	}

	void GLFW_Window::GetSize(int& width, int& height) const
	{
		if (window_)
			glfwGetWindowSize(window_, &width, &height);
		else
			width = height = 0;
	}

	glm::ivec2 GLFW_Window::GetSize() const {
		if (window_) {
			int width = 0;
			int height = 0;
			glfwGetWindowSize(window_, &width, &height);
			return glm::ivec2(width, height);
		}
		return glm::ivec2(0, 0); // or throw, or handle error as needed
	}

	bool GLFW_Window::ShouldClose() const
	{
		return window_ ? glfwWindowShouldClose(window_) : true;
	}

	void GLFW_Window::PollEvents()
	{
		if (window_)
			glfwPollEvents();
		//handle internal input like fullscreen or closing the window
		HandleInternalInput();
	}

	void GLFW_Window::SwapBuffers()
	{
		if (window_)
			glfwSwapBuffers(window_);
	}

	void GLFW_Window::ToggleFullscreen() {
		state_.isFullscreen = !state_.isFullscreen;

		if (state_.isFullscreen) {
			//save current windowed size and position for when we toggle again
			GetSize(state_.windowedWidth, state_.windowedHeight);
			GetPosition(state_.windowedPosX, state_.windowedPosY);
			//get monitor and apply settings
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window_, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else {
			glfwSetWindowMonitor(window_, nullptr, state_.windowedPosX, state_.windowedPosY,
				state_.windowedWidth, state_.windowedHeight, 0);
		}
	}

	void GLFW_Window::TogglePointerLock() {
		state_.isCursorLocked = !state_.isCursorLocked;
		glfwSetInputMode(window_, GLFW_CURSOR,
			state_.isCursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	void GLFW_Window::ToggleVSync() {
		state_.vsyncEnabled = !state_.vsyncEnabled;
		glfwSwapInterval(state_.vsyncEnabled ? 1 : 0);
	}

	void GLFW_Window::SetPosition(int x, int y) {
		glfwSetWindowPos(window_, x, y);
	}

	void GLFW_Window::GetPosition(int& x, int& y) {
		glfwGetWindowPos(window_, &x, &y);
	}

	glm::ivec2 GLFW_Window::GetPosition() const {
		int x, y;
		glfwGetWindowPos(window_, &x, &y);
		return { x, y };
	}

	//basic get or is methods
	bool GLFW_Window::IsFullscreen() const { return state_.isFullscreen; }
	bool GLFW_Window::IsPointerLocked() const { return state_.isCursorLocked; }
	bool GLFW_Window::IsVSyncEnabled() const { return state_.vsyncEnabled; }


}
