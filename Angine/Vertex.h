#pragma once

namespace Angine {

	struct ColorRGBA8 {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
		ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a) {}
		ColorRGBA8() : r(0), g(0), b(0), a(0) {}
	};

	struct Position {
		float x;
		float y;

	};

	struct UV {
		float u;
		float v;
	};

	struct Vertex {

		Position position;

		ColorRGBA8 color;

		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
		void setPos(float x, float y) {
			position.x = x;
			position.y = y;
		}

		bool isSame(Vertex& obj) {
			if (position.x == obj.position.x && position.y == obj.position.y)
				return true;
			return false;
		}
	};

}