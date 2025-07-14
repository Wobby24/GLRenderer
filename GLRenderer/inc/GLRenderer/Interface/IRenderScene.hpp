#pragma once 

enum class SceneID {
    HelloTriangle,
    LightingDemo,
    TexturedQuad,
    // etc
};

class IRenderScene {
public:
    virtual ~IRenderScene() = default;
    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Cleanup() = 0;
    virtual SceneID GetSceneID() const = 0;
};
