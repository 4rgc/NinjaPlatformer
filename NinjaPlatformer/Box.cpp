#include "Header.h"



Box::Box() {

}


Box::~Box() {

}

void Box::Init(BoxDef* def, bool fixedRot, bool isStatic, Angine::GLTexture texture, glm::vec4 uvRect) {
	p_uvRect = uvRect;
	p_texture = texture;
	p_color = def->color;
	p_dims = def->dims;
	p_tex = 1;
	b2BodyDef bodyDef;

	if (isStatic) 
		bodyDef.type = b2_staticBody;
	else
		bodyDef.type = b2_dynamicBody;
	   
	bodyDef.position.Set(def->position.x, def->position.y);
	bodyDef.fixedRotation = fixedRot;

	p_body = def->world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(def->dims.x / 2.0f, def->dims.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	p_fixture = p_body->CreateFixture(&fixtureDef);
}

void Box::Draw(Angine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect;
	destRect.x = p_body->GetPosition().x - p_dims.x / 2.0f;
	destRect.y = p_body->GetPosition().y - p_dims.y / 2.0f;
	destRect.z = p_dims.x;
	destRect.w = p_dims.y;
	spriteBatch.Draw(destRect, p_uvRect, p_texture.ID, 0.0f, p_color, p_body->GetAngle());
}

void Box::Draw(Angine::SpriteBatch & spriteBatch, Angine::TileSheet& tileSheet) {
	glm::vec4 destRect;
	destRect.x = p_body->GetPosition().x - p_dims.x / 2.0f;
	destRect.y = p_body->GetPosition().y - p_dims.y / 2.0f;
	destRect.z = p_dims.x;
	destRect.w = p_dims.y;
	spriteBatch.Draw(destRect, tileSheet.getUV(abs(p_tex)), tileSheet.texture.ID, 0.0f, p_color, p_body->GetAngle());
	if(p_tex > -7)
		p_tex--;
}

void Box::Destroy() {
	p_body->GetWorld()->DestroyBody(p_body);
}
