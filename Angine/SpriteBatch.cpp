#include "angine.h"

namespace Angine {

	Glyph::Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color) :
		texture(texture), depth(depth)
	{

		topLeft.color = color;
		topLeft.setPos(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		topRight.color = color;
		topRight.setPos(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPos(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPos(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	}
	Glyph::Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle) : texture(texture), depth(depth)
	{
		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		//get centered points
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);

		//rotate the points
		tl = RotatePoint(tl, angle) + halfDims;
		bl = RotatePoint(bl, angle) + halfDims;
		br = RotatePoint(br, angle) + halfDims;
		tr = RotatePoint(tr, angle) + halfDims;

		topLeft.color = color;
		topLeft.setPos(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		topRight.color = color;
		topRight.setPos(destRect.x+ tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPos(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPos(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	}

	glm::vec2 Glyph::RotatePoint(glm::vec2 pos, float angle) {
		glm::vec2 newv;
		newv.x = pos.x * cos(angle) - pos.y * sin(angle);
		newv.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newv;
	}

	SpriteBatch::SpriteBatch() : p_vbo(0), p_vao(0) {}


	SpriteBatch::~SpriteBatch() {

	}

	void SpriteBatch::Init() {
		CreateVertexArray();
		
	}

	void SpriteBatch::Begin(GlyphSortType sortType /*GlyphSortType::TEXTURE*/) {
		p_sortType = sortType;
		p_renderBatches.clear();
		p_glyphs.clear();
	}

	void SpriteBatch::End() {
		//set up pointers for sorting
		p_glyphPointers.resize(p_glyphs.size());
		for (int i = 0; i < p_glyphs.size(); i++) {
			p_glyphPointers[i] = &p_glyphs[i];
		}
		SortGlyphs();
		CreateRenderBatches();
	}

	void SpriteBatch::Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color) {
		p_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle) {
		p_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, const glm::vec2& dir) {
		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(dir, right));
		if (dir.y < 0.0f)
			angle = -angle;
		p_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::RenderBatch() {
		glBindVertexArray(p_vao);
		for (int i = 0; i < (int)p_renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, p_renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, p_renderBatches[i].offset, p_renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::CreateRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(p_glyphPointers.size()*6);

		if (p_glyphPointers.empty())
			return;

		int offset = 0;
		int curVertex = 0;
		p_renderBatches.emplace_back(0, 6, p_glyphPointers[0]->texture);

		vertices[curVertex++] = p_glyphPointers[0]->topLeft;
		vertices[curVertex++] = p_glyphPointers[0]->bottomLeft;
		vertices[curVertex++] = p_glyphPointers[0]->bottomRight;
		vertices[curVertex++] = p_glyphPointers[0]->bottomRight;
		vertices[curVertex++] = p_glyphPointers[0]->topRight;
		vertices[curVertex++] = p_glyphPointers[0]->topLeft;
		offset += 6;


		for (int curGlyph = 1; curGlyph < (int)p_glyphPointers.size(); curGlyph++) {
			if (p_glyphPointers[curGlyph]->texture != p_glyphPointers[curGlyph - 1]->texture)
				p_renderBatches.emplace_back(offset, 6, p_glyphPointers[curGlyph]->texture);
			else
				p_renderBatches.back().numVertices += 6;
			vertices[curVertex++] = p_glyphPointers[curGlyph]->topLeft;
			vertices[curVertex++] = p_glyphPointers[curGlyph]->bottomLeft;
			vertices[curVertex++] = p_glyphPointers[curGlyph]->bottomRight;
			vertices[curVertex++] = p_glyphPointers[curGlyph]->bottomRight;
			vertices[curVertex++] = p_glyphPointers[curGlyph]->topRight;
			vertices[curVertex++] = p_glyphPointers[curGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::CreateVertexArray() {

		if (p_vao == 0)
			glGenVertexArrays(1, &p_vao);
		glBindVertexArray(p_vao);
		if (p_vbo == 0)
			glGenBuffers(1, &p_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//position attrib pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
		//color attrib pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
		//uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::SortGlyphs() {
		switch (p_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(p_glyphPointers.begin(), p_glyphPointers.end(), &CompareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(p_glyphPointers.end(), p_glyphPointers.begin(), &CompareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(p_glyphPointers.begin(), p_glyphPointers.end(), &CompareTexture);
			break;
		}
	}
		bool SpriteBatch::CompareFrontToBack(Glyph* a, Glyph *b) {
			return (a->depth < b->depth);
		}
		bool SpriteBatch::CompareBackToFront(Glyph* a, Glyph *b) {
			return (a->depth > b->depth);
		}
		bool SpriteBatch::CompareTexture(Glyph* a, Glyph *b) {
			return (a->texture < b->texture);
		}
}
