#pragma once

namespace Angine {

	class Sprite {
		float p_x;
		float p_y;
		float p_w;
		float p_h;
		GLuint p_vboID;
		GLTexture p_texture;
	public:
		Sprite();
		~Sprite();

		void Init(float x, float y, float w, float h, std::string texturePath);

		void Draw();
	};

}