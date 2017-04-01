#pragma once

class Level;

class Agent {
protected:
	Capsule p_capsule;
	Angine::TileSheet p_texture;
	Level* p_curLvl;
	bool p_direction = 1; // 1 is right, 0 is left
	float p_animTime = 0.0f;
	int p_HP;
public:
	const Capsule& GetCapsule() const { return p_capsule; }
	const b2Vec2& GetPosition() const { return p_capsule.GetBody()->GetPosition(); }
	int GetHP() const { return p_HP; }
	inline bool SubtrHP(int damage) { return ((p_HP -= damage) <= 0); }

	virtual void Init(CapsuleDef* def, Level* thisLvl) = 0;

	virtual void Draw(Angine::SpriteBatch& spriteBatch) = 0;
	inline virtual void DrawDebug(Angine::DebugRenderer& debugRenderer) {
		p_capsule.DrawDebug(debugRenderer);
	}
	inline void Destroy() {
		p_capsule.Destroy();
	}
};