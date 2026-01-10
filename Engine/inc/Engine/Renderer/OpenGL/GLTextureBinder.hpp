#pragma once

#include <memory>
#include <Renderer/OpenGL/GLTexture.hpp>

namespace Renderer {
    namespace OpenGL {

        class GLTextureBinder {
        public:
            static constexpr int MAX_TEXTURE_UNITS = 32;

            GLTextureBinder();

            /**
             * Binds the given texture to the specified texture unit if it's not already bound.
             * @param texture Shared pointer to the GLTexture2D to bind.
             * @param unit Texture unit index (0-based).
             * @return true if the texture was bound (binding occurred), false if skipped.
             */
            bool BindTexture(const std::shared_ptr<GLTexture2D>& texture, int unit);

            /** Resets the internal cache, marking all texture units as unbound. */
            void Reset();

        private:
            // Array storing currently bound texture IDs per texture unit.
            unsigned int boundTextures_[MAX_TEXTURE_UNITS];
        };

    }
} 
