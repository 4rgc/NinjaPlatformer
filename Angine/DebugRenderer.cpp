#include "angine.h"

using namespace Angine;
namespace Angine {
	const char* VERT_SRC = R"(#version 130
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;
    
    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;
    
    fragmentPosition = vertexPosition;
    
    fragmentColor = vertexColor;
})";

	const char* FRAG_SRC = R"(#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})";
}

DebugRenderer::DebugRenderer() {
	//Empty
}

DebugRenderer::~DebugRenderer() {
	Dispose();
}

void DebugRenderer::Init() {
	// Shader init
	p_textureProgram.CompileShadersFromSource(VERT_SRC, FRAG_SRC);
	p_textureProgram.AddAttribute("vertexPosition");
	p_textureProgram.AddAttribute("vertexColor");
	p_textureProgram.LinkShaders();

	// Set up buffers
	glGenVertexArrays(1, &p_vao);
	glGenBuffers(1, &p_vbo);
	glGenBuffers(1, &p_ibo);

	glBindVertexArray(p_vao);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

	glBindVertexArray(0);
}

void DebugRenderer::End() {
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
	// Orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, p_verts.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
	// Upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, p_verts.size() * sizeof(DebugVertex), p_verts.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);
	// Orphan the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	// Upload the data
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, p_indices.size() * sizeof(GLuint), p_indices.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	p_numElements = p_indices.size();
	p_indices.clear();
	p_verts.clear();
}

void DebugRenderer::DrawBox(glm::vec4 destRect, const ColorRGBA8& color, float angle) {

	int i = p_verts.size();
	p_verts.resize(p_verts.size() + 4);

	glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

	// Get points centered at origin
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);

	glm::vec2 positionOffset(destRect.x, destRect.y);

	// Rotate the points
	p_verts[i].position = RotatePoint(tl, angle) + halfDims + positionOffset;
	p_verts[i + 1].position = RotatePoint(bl, angle) + halfDims + positionOffset;
	p_verts[i + 2].position = RotatePoint(br, angle) + halfDims + positionOffset;
	p_verts[i + 3].position = RotatePoint(tr, angle) + halfDims + positionOffset;

	for (int j = i; j < i + 4; j++) {
		p_verts[j].color = color;
	}

	p_indices.reserve(p_indices.size() + 8);

	p_indices.push_back(i);
	p_indices.push_back(i + 1);

	p_indices.push_back(i + 1);
	p_indices.push_back(i + 2);

	p_indices.push_back(i + 2);
	p_indices.push_back(i + 3);

	p_indices.push_back(i + 3);
	p_indices.push_back(i);
}

void DebugRenderer::DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius) {
	static const int NUM_VERTS = 100;
	// Set up vertices
	int start = p_verts.size();
	p_verts.resize(p_verts.size() + NUM_VERTS);
	for (int i = 0; i < NUM_VERTS; i++) {
		float angle = ((float)i / NUM_VERTS) * M_PI * 2.0f;
		p_verts[start + i].position.x = cos(angle) * radius + center.x;
		p_verts[start + i].position.y = sin(angle) * radius + center.y;
		p_verts[start + i].color = color;
	}

	// Set up indices for indexed drawing
	p_indices.reserve(p_indices.size() + NUM_VERTS * 2);
	for (int i = 0; i < NUM_VERTS - 1; i++) {
		p_indices.push_back(start + i);
		p_indices.push_back(start + i + 1);
	}
	p_indices.push_back(start + NUM_VERTS - 1);
	p_indices.push_back(start);
}

void DebugRenderer::Render(const glm::mat4& projectionMatrix, float lineWidth) {
	p_textureProgram.Use();

	GLint pUniform = p_textureProgram.GetUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	glLineWidth(lineWidth);
	glBindVertexArray(p_vao);
	glDrawElements(GL_LINES, p_numElements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	p_textureProgram.UnUse();
}

void DebugRenderer::Dispose() {
	if (p_vao) {
		glDeleteVertexArrays(1, &p_vao);
	}
	if (p_vbo) {
		glDeleteBuffers(1, &p_vbo);
	}
	if (p_ibo) {
		glDeleteBuffers(1, &p_ibo);
	}
	p_textureProgram.Dispose();
}

glm::vec2 DebugRenderer::RotatePoint(glm::vec2 pos, float angle) {
	glm::vec2 newv;
	newv.x = pos.x * cos(angle) - pos.y * sin(angle);
	newv.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newv;
}