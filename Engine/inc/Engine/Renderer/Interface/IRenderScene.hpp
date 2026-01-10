#pragma once

namespace Renderer {

    enum class SceneType {
        HelloTriangle,
        TexturedQuad,
        Quad3D,
        LightingCubes,
        Model3D,
        DepthTesting,
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
        virtual void Render() const = 0;
        virtual void Cleanup() = 0;
        virtual SceneDescriptor GetSceneDescriptor() const = 0;

        // New optional methods with default no-op implementations:
        virtual void OnSurfaceResize(int /*width*/, int /*height*/) {}

    };

}
