#include <Platform/Input/GLFW_InputService.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

namespace Platform {
	namespace Input {

		GLFW_InputService::GLFW_InputService(GLFWwindow* window) : window_(window) {
			// Set the user pointer so the static callback knows this instance
			glfwSetWindowUserPointer(window_, this);

			// Register the scroll callback
			glfwSetScrollCallback(window_, GLFW_InputService::ScrollCallback);

			// Optionally, initialize mouse position
			double x, y;
			glfwGetCursorPos(window_, &x, &y);
			mousePos_ = glm::dvec2(x, y);
		}

		void GLFW_InputService::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
			// Retrieve the GLFW_InputService instance from the window's user pointer
			GLFW_InputService* inputService = reinterpret_cast<GLFW_InputService*>(glfwGetWindowUserPointer(window));
			if (inputService) {
				inputService->scrollDelta_ += static_cast<float>(yoffset);
			}
		}

		bool GLFW_InputService::IsKeyHeld(int key) const {
			InputActionState state = keys_[key].state;
			return state == InputActionState::Held || state == InputActionState::Pressed;
		}

		bool GLFW_InputService::WasKeyPressed(int key) const {
			return keys_[key].state == InputActionState::Pressed;
		}

		bool GLFW_InputService::WasKeyReleased(int key) const {
			return keys_[key].state == InputActionState::Released;
		}

		bool GLFW_InputService::IsActionActive(const std::string& action) const {
			auto it = actionStates_.find(action);
			return (it != actionStates_.end()) ? it->second : false;
		}

		void GLFW_InputService::Update(float deltaTime) {
			for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
				if (keys_[i].state == InputActionState::Pressed)
					keys_[i].state = InputActionState::Held;
				else if (keys_[i].state == InputActionState::Released)
					keys_[i].state = InputActionState::None;
			}

			for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i) {
				if (mouseButtons_[i].state == InputActionState::Pressed)
					mouseButtons_[i].state = InputActionState::Held;
				else if (mouseButtons_[i].state == InputActionState::Released)
					mouseButtons_[i].state = InputActionState::None;
			}

			glm::dvec2 currentMousePos;
			glfwGetCursorPos(window_, &currentMousePos.x, &currentMousePos.y);

			mouseDelta_ = glm::vec2(
				static_cast<float>(currentMousePos.x - mousePos_.x),
				static_cast<float>(currentMousePos.y - mousePos_.y)
			);

			mousePos_ = currentMousePos;

			for (auto& pair : actionMap_) {
				const std::string& action = pair.first;
				int key = pair.second;

				if (IsKeyHeld(key)) {
					 actionStates_[action] = true;
				}
				else {
					 actionStates_[action] = false;
				}
			}


			float scrollThisFrame = scrollDelta_; // read scroll
			scrollDelta_ = 0.0f;                  // reset for next frame
		}

		void GLFW_InputService::BindAction(const std::string& action, int key) {
			actionMap_.emplace(action, key);
		}

		void GLFW_InputService::UnbindAction(const std::string& action, int key) {
			actionMap_.erase(action);
		}

		glm::vec2 GLFW_InputService::GetMousePosition() const
		{
			return glm::vec2(static_cast<float>(mousePos_.x), static_cast<float>(mousePos_.y));
		}

		glm::vec2 GLFW_InputService::GetMouseDelta() const {
			return mouseDelta_;
		}

		float GLFW_InputService::GetScrollDelta() const {
			return scrollDelta_;
		}

		bool GLFW_InputService::IsMouseButtonHeld(int button) const {
			if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
				return false; // out-of-bounds safety check

			InputActionState state = mouseButtons_[button].state;
			return (state == InputActionState::Pressed || state == InputActionState::Held);
		}

		bool GLFW_InputService::WasMouseButtonPressed(int button) const {
			return mouseButtons_[button].state == InputActionState::Pressed;
		}

		bool GLFW_InputService::WasMouseButtonReleased(int button) const {
			return mouseButtons_[button].state == InputActionState::Released;
		}

		void GLFW_InputService::SetCursorVisible(bool visible) {
			cursorVisible_ = visible;
			glfwSetInputMode(window_, GLFW_CURSOR, cursorVisible_ ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
		}
	}
}