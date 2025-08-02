// VertexAttribFlags.hpp
#pragma once

namespace GLRenderer {
    enum class VertexAttribFlags : int {
        NONE = 0,
        POSITION = 1 << 0,
        COLOR = 1 << 1,
        TEXCOORDS = 1 << 2,
    };
}