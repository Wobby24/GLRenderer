#pragma once

#include <GLRenderer/Interface/Lighting/ILightManager.hpp>
#include <unordered_map>
#include <memory>
#include <queue>

namespace GLRenderer {

    class ILightSource; // forward declare
    class GLShader;

    class GLLightManager : public ILightManager {
    public:
        static constexpr int MAX_POINT_LIGHTS = 16;
        static constexpr int MAX_SPOT_LIGHTS = 16;
        static constexpr int MAX_DIR_LIGHTS = 16;

        GLLightManager();
        ~GLLightManager() override;

        void Init() override;
        void Update() override;

        // Unified add method (returns assigned ID)
        int AddLight(std::shared_ptr<ILightSource> light) override;
        void ApplyLights(GLShader& shader);

        void RemoveLight(int id) override;
        void Clear() override;

        int GetNumLights(LightType type) const override;

        std::shared_ptr<ILightSource> GetLight(int id) const;

        const std::unordered_map<int, std::shared_ptr<ILightSource>>& GetAllLights() const {
            return lights_;
        }

    private:
        std::unordered_map<int, std::shared_ptr<ILightSource>> lights_;  // store all lights by ID
        std::queue<int> freeIDs;
        int nextLightID = 0;
    };

}
