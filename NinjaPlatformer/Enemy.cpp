#include "Header.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::Init(CapsuleDef* def, Level* thisLvl) {
	Angine::GLTexture texture = Angine::ResourceManager::GetTexture("Textures/textures/soldier.png");
	p_capsule.Init(def, true);
	p_capsule.GetBody()->SetUserData(this);
	p_texture.Init(texture, glm::ivec2(1, 1));
	p_direction = 0;
	p_moves = 0;
	p_HP = 60;
	p_curLvl = thisLvl;
}

void Enemy::Draw(Angine::SpriteBatch & spriteBatch) {
	glm::vec4 destRect;
	b2Body *body = p_capsule.GetBody();
	destRect.x = body->GetPosition().x - p_capsule.GetDrawDims().x / 2.0f + (p_direction ? 0.2f:-0.2f);
	destRect.y = body->GetPosition().y - p_capsule.GetDims().y / 2.0f;
	destRect.z = p_capsule.GetDrawDims().x;
	destRect.w = p_capsule.GetDrawDims().y; 

	glm::vec4 uvRect = p_texture.getUV(0);

	if (!p_direction) {
		uvRect.x += 1.0f / p_texture.dims.x;
		uvRect.z *= -1;
	}
	spriteBatch.Draw(destRect, uvRect, p_texture.texture.ID, 0.0f, Angine::ColorRGBA8(255, 255, 255, 255), body->GetAngle());

	for (int i = 0; i < p_bullets.size(); i++) {
		p_bullets[i].Draw(spriteBatch);
	}
}

void Enemy::DrawDebug(Angine::DebugRenderer& debugRenderer) {
	p_capsule.DrawDebug(debugRenderer);
	for (int i = 0; i < p_bullets.size(); i++) {
		p_bullets[i].DrawDebug(debugRenderer);
	}
}

void Enemy::Update(Player*& player, std::vector<Agent*>& agents) {
	const float MAX_SPEED = 3.0f;
	if(p_moves)
		Move();
	if (p_capsule.GetBody()->GetLinearVelocity().x < -MAX_SPEED) {
		p_capsule.GetBody()->SetLinearVelocity(b2Vec2(-MAX_SPEED, p_capsule.GetBody()->GetLinearVelocity().y));
	}
	else if (p_capsule.GetBody()->GetLinearVelocity().x > MAX_SPEED) {
		p_capsule.GetBody()->SetLinearVelocity(b2Vec2(MAX_SPEED, p_capsule.GetBody()->GetLinearVelocity().y));
	}
	const float CONST_SHOOT_INTERVAL = 1000;
	const float ENEMY_FOV = 6.0f;

	std::mt19937 randEngine(time(NULL));
	std::uniform_real_distribution<float> randMs(100, 1000);
	
	if (floor(player->GetPosition().y) == floor(GetPosition().y) ||	 ///< If player and enemy are on same height
		ceil(player->GetPosition().y) == floor(GetPosition().y)) {	 /// 
		if (p_direction) {											 ///< If enemy is looking to the right
			if(player->GetPosition().x > GetPosition().x &&			 ///< If player is in front of enemy, not behind
			   player->GetPosition().x - GetPosition().x < ENEMY_FOV)///< And if player is in FOV
				if (p_lastShot.GetMilliseconds() >					 ///< If last shot was more than SHOOT_INTERVAL + randMs ms back
					CONST_SHOOT_INTERVAL + randMs(randEngine)) {	 ///
					Shoot();										 ///< Then Shoot
					p_lastShot.Reset();								 ///< and reset the last shot timer
				}
		}
		else {
			if (player->GetPosition().x < GetPosition().x && 
				GetPosition().x - player->GetPosition().x < ENEMY_FOV)
				if (p_lastShot.GetMilliseconds() > 
					CONST_SHOOT_INTERVAL + randMs(randEngine)) {
					Shoot();
					p_lastShot.Reset();
				}
		}
	}

	//Update the bullets
	for (int i = 0; i < p_bullets.size(); i++) {
		if (!p_bullets[i].Update(agents, player)) {
			p_bullets[i].Destroy();
			p_bullets[i] = p_bullets.back();
			p_bullets.pop_back();
			return;
		}
	}

}

void Enemy::Shoot() {
	Bullet b;
	BulletDef bDef(p_capsule.GetBody()->GetWorld(), GetPosition(), p_direction, 40, 20);
	b.Spawn(&bDef, this);
	p_bullets.push_back(b);
}

void Enemy::Move() {
	if (!p_curLvl->NextGroundBoxExists(b2Vec2(GetPosition().x, GetPosition().y - p_capsule.GetDims().y / 2.0f - SBOX_DIMS / 2.0f), p_direction)) {
		p_direction = !p_direction;
	}
	p_capsule.GetBody()->ApplyForceToCenter(b2Vec2((p_direction?60.0f:-60.0f), 0.0f), 1);
}