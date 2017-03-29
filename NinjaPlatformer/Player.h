#pragma once

enum class PMoveState {
	STANDING = 0x1,
	RUNNING = 0x2,
	PUNCHING = 0x4,
	IN_AIR = 0x8
};

class Level;

class Player {
	Capsule p_capsule;
	Angine::TileSheet p_texture;
	Level* p_curLvl;
	bool p_isGrounded = 0;
	bool p_isPunching = 0;
	bool p_jumped = 0 ;
	bool p_direction = 1; // 1 is right, 0 is left
	float p_animTime = 0.0f;
	int p_framesSinceGrounded;
	int8 p_moveState;
	b2Timer time;

	void OnPunch();
public:
	Player();
	~Player();

	const Capsule& GetCapsule() const { return p_capsule; }

	void Init(CapsuleDef* def, Level* thisLvl);

	void Draw(Angine::SpriteBatch& spriteBatch);
	void DrawDebug(Angine::DebugRenderer& debugRenderer);
	void Update(Angine::InputManager& inputManager);

	const b2Vec2& GetPosition() const { return p_capsule.GetBody()->GetPosition(); }
};