#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "GLTexture.hpp"

namespace GLRenderer {

    class GLTextureCache {
    public:
        static std::shared_ptr<GLTexture2D> LoadOrGet(const std::string& path, TextureType type);

        // Optional: Clear the cache (useful for debugging or reloading)
        static void Clear();

    private:
        static std::unordered_map<std::string, std::weak_ptr<GLTexture2D>> cache_;
    };

} // namespace GLRenderer
