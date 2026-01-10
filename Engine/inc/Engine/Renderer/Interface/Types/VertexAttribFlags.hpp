// VertexAttribFlags.hpp
#pragma once

namespace Renderer {
    enum class VertexAttribFlags : int {
        NONE = 0,
        POSITION = 1 << 0,
        TEXCOORDS = 1 << 1,
        NORMAL = 1 << 2,
    };
}