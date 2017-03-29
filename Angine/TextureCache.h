#pragma once

namespace Angine {

	class TextureCache {
		std::map<std::string, GLTexture> p_textureMap;
	public:
		GLTexture GetTexture(std::string texturePath);
	};

}