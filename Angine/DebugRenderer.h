#pragma once

namespace Angine {

	class DebugRenderer {
	public:
		struct DebugVertex {
			glm::vec2 position;
			ColorRGBA8 color;
		};
	private:
		GLSLProgram p_textureProgram;
		std::vector<DebugVertex> p_verts;
		std::vector<GLuint> p_indices;
		GLuint p_vbo = 0, p_vao = 0, p_ibo = 0;
		int p_numElements = 0;
	public:
		DebugRenderer();
		~DebugRenderer();

		void Init();

		void End();
		void DrawBox(glm::vec4 destRect, const ColorRGBA8& color, float angle);
		void DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
		void Render(const glm::mat4& projectionMatrix, float lineWidth);
		void Dispose();
		glm::vec2 RotatePoint(glm::vec2 pos, float angle);
	};
}