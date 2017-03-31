#pragma once

class Player;

struct BulletDef {
	BulletDef(b2World* world, b2Vec2 position, bool direction, int startDamage, int ttl) :
		world(world),
		position(position),
		direction(direction),
		startDamage(startDamage),
		ttl(ttl)
		{}

	b2World* world;
	b2Vec2 position;
	b2Vec2 dims = b2Vec2(0.05f, 0.05f);
	bool direction;
	int startDamage;
	int ttl;
};

class Bullet {
	Agent* p_parent;
	Box* p_box;
	Angine::GLTexture p_texture;
	bool p_direction; ///< 0 is left, 1 is right
	int p_damage; ///< made it int to be health-damage system more precise
	b2Timer timer;
	int ttl;///< time to live (seconds)
public:
	void Spawn(const BulletDef* def, Agent* parent);

	void Draw(Angine::SpriteBatch& spriteBatch);
	void DrawDebug(Angine::DebugRenderer& debugRenderer);

	bool Update(std::vector<Agent*>& agents, Player*& playerPtr); ///< for checking collisions with agents, returns true if exists

	void Destroy();

	const b2Body* GetBody() const { return p_box->GetBody(); }

};