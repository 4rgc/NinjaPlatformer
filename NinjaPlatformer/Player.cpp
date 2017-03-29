#include "Header.h"

Player::Player() {

}

Player::~Player() {
	
}

void Player::Init(CapsuleDef* def, Level* thisLvl) {
	Angine::GLTexture texture = Angine::ResourceManager::GetTexture("Textures/ninja/blue_ninja.png");
	p_capsule.Init(def, true);
	p_texture.Init(texture, glm::ivec2(10,2));
	p_curLvl = thisLvl;
}

void Player::Draw(Angine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect;
	b2Body *body = p_capsule.GetBody();
	destRect.x = body->GetPosition().x - p_capsule.GetDrawDims().x / 2.0f;
	destRect.y = body->GetPosition().y - p_capsule.GetDims().y / 2.0f;
	destRect.z = p_capsule.GetDrawDims().x;
	destRect.w = p_capsule.GetDrawDims().y;

	int tileIndex;
	int numTiles;
	
	float animationSpeed = 0.1f;
	b2Vec2 velocity = body->GetLinearVelocity();
	
	//Calculate animation
	if (p_isGrounded) {
		if (p_isPunching) {
			numTiles = 4;
			tileIndex = 1;
			if (p_moveState != (int)PMoveState::PUNCHING) {
				p_moveState = (int)PMoveState::PUNCHING;
				p_animTime = 0.0f;
			}
		}
		else if (std::abs(velocity.x) > 1.0f && ((velocity.x > 0 && p_direction) || (velocity.x < 0 && !p_direction))) {
			//running
			numTiles = 6;
			tileIndex = 10;
			animationSpeed = abs(velocity.x) * 0.018f;
			if (p_moveState != (int)PMoveState::RUNNING) {
				p_moveState = (int)PMoveState::RUNNING;
				p_animTime = 0.0f;
			}
		}
		else {
			//standing
			numTiles = 1;
			tileIndex = 0;
			p_moveState &= (int)PMoveState::STANDING;
		}
	}
	else { 
		if(p_isPunching) {
			numTiles = 1;
			tileIndex = 18;
			animationSpeed *= 0.25f;
			if (p_moveState != (int)PMoveState::PUNCHING) {
				p_moveState = (int)PMoveState::PUNCHING;
				p_animTime = 0.0f;
			}
		}
		else if (abs(velocity.x) > 8.0f) {
			numTiles = 1;
			tileIndex = 10;
		}
		else if (velocity.y <= 0.0f) {
			//falling
			numTiles = 1;
			tileIndex = 17;
			p_moveState = (int)PMoveState::IN_AIR;
		}
		else {
			//rising
			numTiles = 1;
			tileIndex = 16;
			p_moveState = (int)PMoveState::IN_AIR;
		}
	}

	//Increment animation time
	p_animTime += animationSpeed;

	//Check for punch end
	if (p_animTime > numTiles) {
		p_isPunching = 0;
	}

	//Apply animation
	tileIndex = tileIndex + (int)p_animTime % numTiles;

	glm::vec4 uvRect = p_texture.getUV(tileIndex);


	if (!p_direction) {
		uvRect.x += 1.0f / p_texture.dims.x;
		uvRect.z *= -1;
	}

	spriteBatch.Draw(destRect, uvRect, p_texture.texture.ID, 0.0f, Angine::ColorRGBA8(255,255,255,255), body->GetAngle());
}

void Player::DrawDebug(Angine::DebugRenderer& debugRenderer) {
	p_capsule.DrawDebug(debugRenderer);
}

void Player::Update(Angine::InputManager& inputManager) {
	float MAX_SPEED = 8.0f;
	b2Body* body = p_capsule.GetBody();
	if (inputManager.IsKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), 1);
		p_direction = 0;
	}
	else if(inputManager.IsKeyDown(SDLK_d)){
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
		p_direction = 1;
	}
	else {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95, body->GetLinearVelocity().y));
	}
	if (inputManager.IsKeyPressed(SDLK_LSHIFT)) {
		p_isPunching = 1;
		OnPunch();
	}
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}
	p_isGrounded = 0;
	//Loop througn the contact points
	bool below = false;
	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* c = ce->contact;
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);
			//Check if points are below
			below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - p_capsule.GetDims().y / 2.0f + p_capsule.GetDims().x / 2.0f + 0.01f && 
					manifold.points[i].x < body->GetPosition().x + p_capsule.GetDims().x / 2.0f - 0.01f &&
					manifold.points[i].x > body->GetPosition().x - p_capsule.GetDims().x / 2.0f + 0.01f) {
					below = true;
					p_isGrounded = 1;
					if(time.GetMilliseconds() > 100.0f)
					p_jumped = 0;
					time.Reset();
					break;
				}
			}
		}
	}
	//You can jump 200ms after you got off the ground
	if ((p_isGrounded || time.GetMilliseconds() < 200.0f) && p_jumped == false) {
		//We can jump
		if (inputManager.IsKeyPressed(SDLK_SPACE)) {
			p_jumped = 1;
			body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 30.f), true);
		}
	}
}

void Player::OnPunch() {
	std::vector<Box*> nearBoxes = p_curLvl->IsNearBox(p_capsule.GetBody()->GetPosition());
	if (nearBoxes.empty())
		return;
	for (int i = 0; i < nearBoxes.size(); i++) {
		if (nearBoxes[i]->GetTileID() == 1) {
			if (p_direction) {
				if (nearBoxes[i]->GetBody()->GetPosition().x > p_capsule.GetBody()->GetPosition().x) {
					nearBoxes[i]->GetTileID()--;
					nearBoxes[i]->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.5f, 0.0f), true);
				}
			}
			else {
				if (nearBoxes[i]->GetBody()->GetPosition().x < p_capsule.GetBody()->GetPosition().x) {
					nearBoxes[i]->GetTileID()--;
					nearBoxes[i]->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-0.5f, 0.0f), true);
				}
			}
		}
	}
}