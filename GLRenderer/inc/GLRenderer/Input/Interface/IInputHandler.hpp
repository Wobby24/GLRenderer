#pragma once

namespace GLRenderer::Input {

    class IInputHandler {
    public:
        virtual ~IInputHandler() = default;

        virtual void Update() = 0;

        virtual bool IsKeyPressed(int key) const = 0;
        virtual bool IsKeyReleased(int key) const = 0;
        virtual bool IsKeyDown(int key) const = 0;

        virtual void GetMousePosition(double& x, double& y) const = 0;
        virtual void SetMousePosition(double x, double y) = 0;

        // Scroll offsets accumulated since last Update()
        virtual double GetScrollX() const = 0;
        virtual double GetScrollY() const = 0;

        // Reset scroll offsets (usually called at the start/end of Update)
        virtual void ResetScroll() = 0;
    };

} // namespace GLRenderer::Input
