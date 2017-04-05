#include "Header.h"
#include <glm/gtx/rotate_vector.hpp>


void Bullet::Spawn(const BulletDef* def, Agent* parent) {
	p_box = new Box();
	BoxDef bDef(def->world, b2Vec2(def->position.x + (def->direction ? 1.0f : -1.0f), def->position.y), b2Vec2(0.5f, 0.5f));
	p_box->Init(&bDef, false, false, Angine::ResourceManager::GetTexture("Textures/textures/bullet.png"));
	p_direction = def->direction;
	std::mt19937 randEngine((unsigned int)time(NULL));
	std::uniform_real_distribution<float> angle((float)-M_PI / 18.0f, (float)M_PI / 18.0f);
	p_box->GetBody()->SetTransform(p_box->GetPosition(), (float32)M_PI);
	p_damage = def->startDamage;
	ttl = def->ttl;
	timer.Reset();
	p_parent = parent;
	b2Vec2 impulse = (p_direction ? b2Vec2(13.0f, 0.0f) : b2Vec2(-13.0f, 0.0f));
	p_box->GetBody()->ApplyLinearImpulseToCenter(impulse, true);
}

void Bullet::Draw(Angine::SpriteBatch& spriteBatch) {
	if (p_box->GetBody())
		p_box->Draw(spriteBatch);
}

void Bullet::DrawDebug(Angine::DebugRenderer& debugRenderer) {
	glm::vec4 destRect;
	destRect.x = p_box->GetBody()->GetPosition().x - p_box->GetDims().x / 2.0f;
	destRect.y = p_box->GetBody()->GetPosition().y - p_box->GetDims().y / 2.0f;
	destRect.w = p_box->GetDims().x;
	destRect.z = p_box->GetDims().y;
	debugRenderer.DrawBox(destRect, Angine::ColorRGBA8(128, 255, 128, 255), p_box->GetBody()->GetAngle());
}

bool Bullet::Update(std::vector<Agent*>& agents, Player*& playerPtr) {
	if (!p_box->GetBody())
		return false;
	p_damage -= 1;
	if (b2ContactEdge* ce = p_box->GetBody()->GetContactList()) {
		bool touching = false;
		for (ce; ce; ce = ce->next) {
			if (ce->contact->IsTouching()) {
				//touching = true;
				for (size_t i = 0; i < agents.size(); i++) {
					//If we are on parent, continue looping
					if (agents[i]->GetCapsule().GetBody()->GetUserData() == p_parent->GetCapsule().GetBody()->GetUserData()) {
						continue;
					}
					if (agents[i]->GetCapsule().GetBody()->GetUserData() == 
						ce->contact->GetFixtureB()->GetBody()->GetUserData()) {
						//TODO: instead of destroying an agent, just subtract damage from HP
						if (agents[i]->SubtrHP(p_damage)) {
							if (i == 0)
								playerPtr = nullptr;
							agents[i]->Destroy();
							delete agents[i];
							agents[i] = agents.back();
							agents.pop_back();
						}
						break;
					}
				}
				return false;
				break;
			}
		}
	}
	return true;
}

void Bullet::Destroy() {
	p_box->Destroy();
	p_box = nullptr;
}