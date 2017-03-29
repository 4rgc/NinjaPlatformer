#pragma once

namespace Angine {

	class TileSheet {
	public:
		GLTexture texture;
		glm::ivec2 dims;
		glm::vec4 getUV(int index) {
			int xTile = index % dims.x;
			int yTile = index / dims.x;

			glm::vec4 uvs;
			uvs.x = xTile / (float)dims.x;
			uvs.y = yTile / (float)dims.y;
			uvs.z = 1.0f / dims.x;
			uvs.w = 1.0f / dims.y;

			return uvs;
		}
		void Init(const GLTexture& texture, glm::ivec2& tileDims) {
			this->texture = texture;
			this->dims = tileDims;
		}

	};
}