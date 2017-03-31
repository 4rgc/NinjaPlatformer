#pragma once

class Enemy :public Agent {
	b2Timer p_lastShot;
	std::vector<Bullet> p_bullets;
	//void Move(); TO BE IMPLEMENTED
	void Shoot();
public:
	Enemy();
	~Enemy();

	void Init(CapsuleDef* def, Level* thisLvl) override;

	void Draw(Angine::SpriteBatch& spriteBatch) override;
	void DrawDebug(Angine::DebugRenderer& debugRenderer) override;

	void Update(Player*& player, std::vector<Agent*>& agents);

};

