#pragma once

struct CapsuleDef {
	CapsuleDef(b2World* world) : world(world) {}
	CapsuleDef(b2World* world, const b2Vec2& pos) : world(world), position(pos) {}
	CapsuleDef(b2World* world, const b2Vec2& pos, const b2Vec2& dims) : world(world), position(pos), dims(dims) {}
	CapsuleDef(b2World* world, const b2Vec2& pos, const b2Vec2& dims, float density, float friction) : 
		world(world), position(pos), dims(dims), density(density), friction(friction) {}
	b2World* world = nullptr;
	b2Vec2 position = b2Vec2(0.0f, 0.0f);
	b2Vec2 dims = b2Vec2(5.0f, 5.0f);
	glm::vec2 drawDims;
	float density = 1.0f;
	float friction = 0.3f;
};

class Capsule {
	b2Body* p_body = nullptr;
	b2Fixture* p_fixtures[3];
	b2Vec2 p_dims;
	glm::vec2 p_drawDims;
	glm::vec4 p_uvRect;
public:
	b2Body* GetBody() const { return p_body; }
	b2Fixture* GetFixture(int i) const { return p_fixtures[i]; }
	const b2Vec2& GetDims() const { return p_dims; }
	const glm::vec2& GetDrawDims() const { return p_drawDims; }

	void Init(CapsuleDef* def, bool fixedRot);

	void DrawDebug(Angine::DebugRenderer& debugRenderer);

	void Destroy() {
		p_body->GetWorld()->DestroyBody(p_body);
		p_body = nullptr;
	}
};

