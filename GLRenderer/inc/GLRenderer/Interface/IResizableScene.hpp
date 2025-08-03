#pragma once

class IResizableScene {
public:
    virtual ~IResizableScene() = default;
    virtual void OnWindowResize(int width, int height) = 0;
};
