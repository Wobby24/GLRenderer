#pragma once

namespace Renderer {
    class ITexture {
    public:
        virtual ~ITexture() = default; // Or = 0; if it's pure virtual
        virtual void loadTexture() = 0;
    };
};
