#include "angine.h"

namespace Angine {

	Sprite::Sprite() {
		p_vboID = 0;
	}

	Sprite::~Sprite() {
		if (p_vboID != 0)
			glDeleteBuffers(1, &p_vboID);
	}

	void Sprite::Init(float x, float y, float w, float h, std::string texturePath) {
		p_x = x;
		p_y = y;
		p_w = w;
		p_h = h;

		p_texture = ResourceManager::GetTexture(texturePath);

		if (p_vboID == 0)
			glGenBuffers(1, &p_vboID);

		Vertex vertexData[6];

		//1st half of the screen
		//1st triangle
		vertexData[0].setPos(x + w, y + h);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPos(x, y + h);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPos(x, y);
		vertexData[2].setUV(0.0f, 0.0f);
		//2nd triangle
		vertexData[3].setPos(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPos(x + w, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPos(x + w, y + h);
		vertexData[5].setUV(1.0f, 1.0f);

		for (int i = 0; i < 6; i++) {
			vertexData[i].setColor(255, 0, 255, 255);
		}

		//(0,0)
		vertexData[0].setColor(255, 0, 0, 255);
		vertexData[5].setColor(255, 0, 0, 255);

		vertexData[2].setColor(255, 165, 0, 255);
		vertexData[3].setColor(255, 165, 0, 255);

		vertexData[1].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 255, 255);

		glBindBuffer(GL_ARRAY_BUFFER, p_vboID);
		//			  buffer type		//buffer size//	  //buffer ref   //usage
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Sprite::Draw() {

		glBindTexture(GL_TEXTURE_2D, p_texture.ID);
		glBindBuffer(GL_ARRAY_BUFFER, p_vboID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//position attrib pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
		//color attrib pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
		//uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}