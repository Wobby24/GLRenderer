#pragma once

#include <glm/glm.hpp>
#include <string>
namespace Platform {
    namespace Input {
        // Generic input action type
        enum class InputActionState {
            None,
            Pressed,    // Pressed this frame
            Held,       // Currently held down
            Released    // Released this frame
        };

        class IInputService {
        public:
            virtual ~IInputService() = default;

            // ---------------------
            // Keyboard
            // ---------------------
            virtual bool IsKeyHeld(int key) const = 0;
            virtual bool WasKeyPressed(int key) const = 0;

            // Action mapping (optional)
            virtual bool IsActionActive(const std::string& action) const = 0;
            virtual void BindAction(const std::string& action, int key) = 0;
            virtual void UnbindAction(const std::string& action, int key) = 0;


            // ---------------------
            // Mouse
            // ---------------------
            virtual glm::vec2 GetMousePosition() const = 0;
            virtual glm::vec2 GetMouseDelta() const = 0;
            virtual float GetScrollDelta() const = 0;
            virtual bool IsMouseButtonHeld(int button) const = 0;
            virtual bool WasMouseButtonPressed(int button) const = 0;
            virtual void SetCursorVisible(bool visible) = 0;
            virtual bool WasKeyReleased(int key) const = 0;
            virtual bool WasMouseButtonReleased(int button) const = 0;

            // ---------------------
            // Frame update
            // ---------------------
            virtual void Update(float deltaTime) = 0;
        };
    }
}