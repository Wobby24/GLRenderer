#pragma once

namespace GLRenderer {
	struct Vertex {
		float position[3];  // x, y, z
		float texCoord[2];  // u, v
		float normal[3];    // nx, ny, nz
		Vertex(float x, float y, float z, float u = 0.0f, float v = 0.0f, float nx = 0.0f, float ny = 0.0f, float nz = 1.0f)
			: position{ x, y, z }, texCoord{ u, v }, normal{ nx, ny, nz } {
		}
	};
}
