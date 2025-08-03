#pragma once 

enum class SceneType {
    HelloTriangle,
    TexturedQuad,
    Quad3D,
    // etc
};

struct SceneDescriptor {
    SceneType type;
    int id;
};

class IRenderScene {
public:
    virtual ~IRenderScene() = default;
    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Cleanup() = 0;
    virtual SceneDescriptor GetSceneDescriptor() const = 0;
};
