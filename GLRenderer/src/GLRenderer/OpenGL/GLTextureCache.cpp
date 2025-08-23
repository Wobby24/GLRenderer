#include "GLRenderer/OpenGL/GLTextureCache.hpp"

namespace GLRenderer {

    std::unordered_map<std::string, std::weak_ptr<GLTexture2D>> GLTextureCache::cache_;

    std::shared_ptr<GLTexture2D> GLTextureCache::LoadOrGet(const std::string& path, TextureType type) {
        auto it = cache_.find(path);
        if (it != cache_.end()) {
            if (auto tex = it->second.lock()) {
                return tex; // Reuse existing texture
            }
        }

        auto newTexture = std::make_shared<GLTexture2D>(path, type);
        newTexture->loadTexture();
        cache_[path] = newTexture;
        return newTexture;
    }

    void GLTextureCache::Clear() {
        cache_.clear();
    }

}
