#pragma once

namespace Angine {
	class Animator {
		SpriteBatch p_spriteBatch;
		GLTexture p_texture;
		int p_numTiles;
	public:
		Animator();
		~Animator();
	};

}