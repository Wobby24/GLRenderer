#pragma once

namespace Utils {
    namespace Math {
        // Linear interpolation
        template <typename T>
        T lerp(const T& a, const T& b, float t) {
            static_assert(std::is_arithmetic_v<T> || std::is_class_v<T>,
                "lerp requires arithmetic or class type supporting operators +, -, *");

            return a + (b - a) * t;
        }
    }
}
