#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <Platform/Input/Interface/IInputService.hpp>

namespace Platform {
	namespace Input {
		class GLFW_InputService final : public IInputService {
        public:
            explicit GLFW_InputService(GLFWwindow* window);

            // ---------------------
            // Keyboard
            // ---------------------
            bool IsKeyHeld(int key) const override;
            bool WasKeyPressed(int key) const override;
            bool IsActionActive(const std::string& action) const override;
            void BindAction(const std::string& action, int key) override;
            void UnbindAction(const std::string& action, int key) override;

            // ---------------------
            // Mouse
            // ---------------------
            glm::vec2 GetMousePosition() const override;
            glm::vec2 GetMouseDelta() const override;
            float GetScrollDelta() const override;
            bool IsMouseButtonHeld(int button) const override;
            bool WasMouseButtonPressed(int button) const override;
            void SetCursorVisible(bool visible) override;
            bool WasKeyReleased(int key) const override;
            bool WasMouseButtonReleased(int button) const override;


            // ---------------------
            // Frame update
            // ---------------------
            void Update(float deltaTime) override;

        private:
            GLFWwindow* window_;

            // -------------------------
         // Internal state
         // -------------------------
            struct KeyState {
                InputActionState state = InputActionState::None;
            };

            KeyState keys_[GLFW_KEY_LAST + 1];             // all keyboard keys
            KeyState mouseButtons_[GLFW_MOUSE_BUTTON_LAST + 1];  // all mouse buttons
            glm::dvec2 mousePos_{ 0.0, 0.0 }; // initialize with doubles
            glm::vec2 mouseDelta_ = glm::vec2(0.0f);      // movement since last frame
            float scrollDelta_ = 0.0f;                     // scroll since last frame
            bool cursorVisible_;

            std::unordered_map<std::string, int> actionMap_;  // action name -> key binding
            std::unordered_map<std::string, bool> actionStates_; // tracks if action is active

            static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		};
	}
}