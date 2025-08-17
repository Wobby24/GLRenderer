#pragma once 

#include <GLRenderer/Interface/Lighting/ILightSource.hpp>
#include <memory>

namespace GLRenderer {

    class ILightManager {
    public:
        virtual ~ILightManager() = 0;

        virtual void Init() = 0;
        virtual void Update() = 0;

        // Add light returns assigned unique ID
        virtual int AddLight(std::shared_ptr<ILightSource> light) = 0;

        // Remove light by ID
        virtual void RemoveLight(int id) = 0;

        virtual void Clear() = 0;

        virtual int GetNumLights(LightType type) const = 0;
    };

    inline ILightManager::~ILightManager() {}
}
