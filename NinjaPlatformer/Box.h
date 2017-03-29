#pragma once

struct BoxDef {
	BoxDef(b2World* world) : world(world) {}
	BoxDef(b2World* world, const b2Vec2& pos) : world(world), position(pos) {}
	BoxDef(b2World* world, const b2Vec2& pos, const b2Vec2& dims) : world(world), position(pos), dims(dims){}
	b2World* world = nullptr;
	b2Vec2 position = b2Vec2(0.0f, 0.0f);
	b2Vec2 dims = b2Vec2(5.0f, 5.0f);
	Angine::ColorRGBA8 color = Angine::ColorRGBA8(255,255,255,255);
};

class Box {
	b2Body* p_body = nullptr;
	b2Fixture* p_fixture = nullptr;
	b2Vec2 p_dims;
	Angine::ColorRGBA8 p_color;
	Angine::GLTexture p_texture;
	glm::vec4 p_uvRect;
	int p_tex; ///< for destroying animation (1 - normal)
public:
	Box();
	~Box();

	b2Body* GetBody() const { return p_body; }
	b2Fixture* GetFixture() const { return p_fixture; }
	const b2Vec2& GetDims() const { return p_dims; }
	const Angine::ColorRGBA8& GetColor() const { return p_color; }
	void SetColor(Angine::ColorRGBA8& color) { p_color = color; }
	int &GetTileID() { return p_tex; }
	const b2Vec2& GetPosition() { return p_body->GetPosition(); }

	void Init(BoxDef* def, bool fixedRot, bool isStatic, Angine::GLTexture texture, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void Draw(Angine::SpriteBatch& spriteBatch);
	void Draw(Angine::SpriteBatch& spriteBatch, Angine::TileSheet& tileSheet);

	void Destroy();
};

