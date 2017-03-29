#include "Header.h"

void Capsule::Init(CapsuleDef* def, bool fixedRot) {
	p_dims = def->dims;
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(def->position.x, def->position.y);
	bodyDef.fixedRotation = fixedRot;
	p_drawDims = def->drawDims;

	p_body = def->world->CreateBody(&bodyDef);

	//Create the box
	b2PolygonShape boxShape;
	boxShape.SetAsBox(def->dims.x / 2.0f - 0.03f, (def->dims.y - def->dims.x) / 2.0f - 0.03f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = def->density;
	fixtureDef.friction = def->friction;
	p_fixtures[0] = p_body->CreateFixture(&fixtureDef);

	//Create the circles
	
	b2CircleShape circleShape;
	circleShape.m_radius = (p_dims.x / 2.0f) + 0.01f;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = def->density;
	circleDef.friction = def->friction;
	
	//Bottom circle
	circleShape.m_p.Set(0.0f, (-p_dims.y + p_dims.x) / 2.0f);
	p_fixtures[1] = p_body->CreateFixture(&circleDef);
	//Top circle
	circleShape.m_p.Set(0.0f, (p_dims.y - p_dims.x) / 2.0f - 0.02f);
	p_fixtures[2] = p_body->CreateFixture(&circleDef);
}

void Capsule::DrawDebug(Angine::DebugRenderer& debugRenderer) {
	glm::vec4 destRect;
	destRect.x = p_body->GetPosition().x - p_dims.x / 2.0f;
	destRect.y = p_body->GetPosition().y - (p_dims.y - p_dims.x) / 2.0f;
	destRect.z = p_dims.x - 0.03f;
	destRect.w = p_dims.y - p_dims.x - 0.03f;
	debugRenderer.DrawBox(destRect, Angine::ColorRGBA8(255, 255, 255, 255), p_body->GetAngle());

	//Draw circles
	debugRenderer.DrawCircle(glm::vec2(destRect.x + p_dims.x / 2, destRect.y), Angine::ColorRGBA8(255,255,255,255), (p_dims.x / 2.0f) + 0.01f);
	debugRenderer.DrawCircle(glm::vec2(destRect.x + p_dims.x / 2, destRect.y + destRect.w), Angine::ColorRGBA8(255, 255, 255, 255), p_dims.x / 2.0f);
}
