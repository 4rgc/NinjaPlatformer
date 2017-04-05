#pragma once

class Light {
public:
	Angine::ColorRGBA8 color;
	b2Vec2 position;
	float size;

	void Draw(Angine::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;
		destRect.x = position.x - size / 2.0f;
		destRect.y = position.y - size / 2.0f;
		destRect.z = size;
		destRect.w = size;
		spriteBatch.Draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), GLuint(0), 0.0f, color, 0);
	}

};