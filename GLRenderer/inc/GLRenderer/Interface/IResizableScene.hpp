#pragma once
#include "IRenderScene.hpp"

class IResizableScene :public IRenderScene {
public:
    virtual ~IResizableScene() = default;
    virtual void OnWindowResize(int width, int height) = 0;
};
