#pragma once

namespace Angine {

	class ResourceManager {
		static TextureCache p_textureCache;
	public:
		static GLTexture GetTexture(std::string texturePath);
	};

}