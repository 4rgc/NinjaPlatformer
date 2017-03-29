#pragma once

namespace Angine {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph {
		glm::vec2 RotatePoint(glm::vec2 pos, float angle);
	public:
		Glyph() {}
		Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color);
		Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle);

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : 
			offset(offset),
			numVertices(numVertices),
			texture(texture) {}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch {
		static bool CompareFrontToBack(Glyph* a, Glyph *b);
		static bool CompareBackToFront(Glyph* a, Glyph *b);
		static bool CompareTexture(Glyph* a, Glyph *b);
		void SortGlyphs();
		void CreateRenderBatches();
		void CreateVertexArray();
		
		GlyphSortType p_sortType;
		
		GLuint p_vbo;
		GLuint p_vao;

		std::vector<Glyph*> p_glyphPointers; ///< these are for sorting
		std::vector<Glyph> p_glyphs; ///< these are for storing
		std::vector<RenderBatch> p_renderBatches;
	public:
		SpriteBatch();
		~SpriteBatch();

		void Init();
		void Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void End();

		void Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color);

		void Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle);

		void Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, const glm::vec2& dir);
		
		void RenderBatch();
	};

}