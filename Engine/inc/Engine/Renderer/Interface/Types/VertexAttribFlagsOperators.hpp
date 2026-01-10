#pragma once

#include <Renderer/Interface/Types/VertexAttribFlags.hpp>

namespace Renderer {
    // Enable bitwise operators
    inline VertexAttribFlags operator|(VertexAttribFlags a, VertexAttribFlags b) {
        return static_cast<VertexAttribFlags>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline VertexAttribFlags operator&(VertexAttribFlags a, VertexAttribFlags b) {
        return static_cast<VertexAttribFlags>(static_cast<int>(a) & static_cast<int>(b));
    }

    inline VertexAttribFlags operator~(VertexAttribFlags a) {
        return static_cast<VertexAttribFlags>(~static_cast<int>(a));
    }

    // Optionally add compound assignment if needed
    inline VertexAttribFlags& operator|=(VertexAttribFlags& a, VertexAttribFlags b) {
        a = a | b;
        return a;
    }

    inline bool HasFlag(VertexAttribFlags value, VertexAttribFlags flag) {
        return (static_cast<int>(value) & static_cast<int>(flag)) == static_cast<int>(flag);
    }

}
