#include "GLRenderer/OpenGL/Lighting/GLLightManager.hpp"
#include <GLRenderer/Interface/Lighting/ILightSource.hpp>  // Needed for getType()
#include <GLRenderer/OpenGL/GLShader.hpp>
#include <iostream>

namespace GLRenderer {

    GLLightManager::GLLightManager() = default;
    GLLightManager::~GLLightManager() = default;

    void GLLightManager::Init() {
        // Optional: Initialize anything if needed
    }

    void GLLightManager::Update() {
        // Optional: Update lights per frame
    }

    void GLLightManager::ApplyLights(GLShader& shader) {
        int pointIndex = 0;
        int spotIndex = 0;
        int dirIndex = 0;

        for (const auto& [id, light] : lights_) {
            if (!light) continue;

            switch (light->getType()) {
            case LightType::Point:
                light->applyProperties(shader, pointIndex++);
                break;
            case LightType::Spot:
                light->applyProperties(shader, spotIndex++);
                break;
            case LightType::Directional:
                light->applyProperties(shader, dirIndex++);
                break;
            default:
                break;
            }
        }
    }

    int GLLightManager::AddLight(std::shared_ptr<ILightSource> light) {
        if (!light) return -1;

        int count = GetNumLights(light->getType());

        switch (light->getType()) {
        case LightType::Point:
            if (count >= MAX_POINT_LIGHTS) {
                std::cerr << "Max point lights reached!\n";
                return -1;
            }
            break;
        case LightType::Spot:
            if (count >= MAX_SPOT_LIGHTS) {
                std::cerr << "Max spot lights reached!\n";
                return -1;
            }
            break;
        case LightType::Directional:
            if (count >= MAX_DIR_LIGHTS) {
                std::cerr << "Max directional lights reached!\n";
                return -1;
            }
            break;
        default:
            std::cerr << "Unknown light type!\n";
            return -1;
        }

        // Use a freed ID if available, else use the next incremented ID
        int assignedID;
        if (!freeIDs.empty()) {
            assignedID = freeIDs.front();
            freeIDs.pop();
        }
        else {
            assignedID = nextLightID++;
        }

        lights_[assignedID] = std::move(light);
        return assignedID;
    }

    void GLLightManager::RemoveLight(int id) {
        auto it = lights_.find(id);
        if (it != lights_.end()) {
            lights_.erase(it);
            freeIDs.push(id); // Recycle this ID
        }
    }

    void GLLightManager::Clear() {
        lights_.clear();
        nextLightID = 0;

        // Clear recycled IDs
        std::queue<int> empty;
        std::swap(freeIDs, empty);
    }

    int GLLightManager::GetNumLights(LightType type) const {
        int count = 0;
        for (const auto& [id, light] : lights_) {
            if (light && light->getType() == type) {
                ++count;
            }
        }
        return count;
    }

    std::shared_ptr<ILightSource> GLLightManager::GetLight(int id) const {
        auto it = lights_.find(id);
        return it != lights_.end() ? it->second : nullptr;
    }

} // namespace GLRenderer
