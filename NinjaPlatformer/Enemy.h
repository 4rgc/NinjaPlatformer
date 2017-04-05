#pragma once

class Enemy :public Agent {
	b2Timer p_lastShot;
	std::vector<Bullet> p_bullets;
	bool p_moves;
	void Move();
	void Shoot();
public:
	Enemy();
	~Enemy();

	void Init(CapsuleDef* def, Level* thisLvl) override;

	void Draw(Angine::SpriteBatch& spriteBatch) override;
	void DrawDebug(Angine::DebugRenderer& debugRenderer) override;

	void Update(Player*& player, std::vector<Agent*>& agents);

	void SetDir(bool dir) { p_direction = dir; }
	void SetMoves(bool moves) { p_moves = moves; }
};

