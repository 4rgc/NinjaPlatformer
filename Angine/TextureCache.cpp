#include "angine.h"

namespace Angine {

	GLTexture TextureCache::GetTexture(std::string texturePath) {
		//lookup the texture and see if its in the map
		auto mit = p_textureMap.find(texturePath);

		if (mit == p_textureMap.end()) {
			//load the texture
			GLTexture newTexture = ImageLoader::LoadPNG(texturePath);
			//insert it into the map
			p_textureMap.insert(make_pair(texturePath, newTexture));
			//std::cout << "Loaded a new texture\n";
			return newTexture;
		}
		//std::cout << "Loaded cached texture\n";
		return mit->second;
	}
}