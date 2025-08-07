#include <GLRenderer/Window/Window.hpp>
#include <string>
#include <stdexcept>
#include <GLFW/glfw3.h> // Needed for GLFW calls

namespace GLRenderer::Window
{
	Window::Window(int width, int height, const std::string& title, int glMajorVersion, int glMinorVersion)
		: width_(width), height_(height), title_(title)
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

	void Window::processInput()
	{
		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window_, true);
	}

	Window::~Window()
	{
		if (isCleanedUp_) return;

		glfwDestroyWindow(window_);
		glfwTerminate();
		isCleanedUp_ = true;
	}

	void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		(void)window; // Silence unused parameter warning
		glViewport(0, 0, width, height);
	}

	void Window::SetTitle(const std::string& title)
	{
		title_ = title;
		if (window_)
			glfwSetWindowTitle(window_, title.c_str());
	}
	
	void Window::SetSize(int width, int height)
	{
		width_ = width;
		height_ = height;
		if (window_)
			glfwSetWindowSize(window_, width_, height_);
	}

	void Window::GetSize(int& width, int& height) const
	{
		if (window_)
			glfwGetWindowSize(window_, &width, &height);
		else
			width = height = 0;
	}

	glm::ivec2 Window::GetSize() const {
		if (window_) {
			int width = 0;
			int height = 0;
			glfwGetWindowSize(window_, &width, &height);
			return glm::ivec2(width, height);
		}
		return glm::ivec2(0, 0); // or throw, or handle error as needed
	}

	bool Window::ShouldClose() const
	{
		return window_ ? glfwWindowShouldClose(window_) : true;
	}

	void Window::PollEvents()
	{
		if (window_)
			glfwPollEvents();
		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window_, true);
	}

	void Window::SwapBuffers()
	{
		if (window_)
			glfwSwapBuffers(window_);
	}

}
