// VertexAttribFlags.hpp
#pragma once

namespace GLRenderer {
    enum class VertexAttribFlags : int {
        NONE = 0,
        POSITION = 1 << 0,
        TEXCOORDS = 1 << 1,
        COLOR = 1 << 2,
    };
}