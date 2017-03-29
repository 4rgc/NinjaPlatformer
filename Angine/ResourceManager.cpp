#include "angine.h"

namespace Angine {

	TextureCache ResourceManager::p_textureCache = {};

	GLTexture ResourceManager::GetTexture(std::string texturePath) {
		return p_textureCache.GetTexture(texturePath);
	}

}