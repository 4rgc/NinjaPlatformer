#pragma once

enum class PMoveState {
	STANDING = 0x1,
	RUNNING = 0x2,
	PUNCHING = 0x4,
	IN_AIR = 0x8
};

class Level;
class Bullet;

class Player : public Agent {
	bool p_isGrounded = 0;
	bool p_isPunching = 0;
	bool p_jumped = 0 ;
	int p_framesSinceGrounded;
	PMoveState p_moveState;
	b2Timer time;

	void OnPunch(std::vector<Agent*>& agents);
public:
	Player();
	~Player();

	void Init(CapsuleDef* def, Level* thisLvl) override;

	void Draw(Angine::SpriteBatch& spriteBatch) override;
	void Update(Angine::InputManager& inputManager, std::vector<Agent*>& agents);
};