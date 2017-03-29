#include "angine.h"

using namespace Angine;

ParticleBatch2D::ParticleBatch2D() {
	//empty
}


ParticleBatch2D::~ParticleBatch2D() {
	delete[] p_particles;
}

void ParticleBatch2D::Init(int maxParticles, float decayRate, GLTexture& texture, std::function<void(Particle2D&, float)> updateFunc) {
	p_maxParticles = maxParticles;
	p_particles = new Particle2D[maxParticles];
	p_decayRate = decayRate;
	p_texture = texture;
	p_updateFunc = updateFunc;
}

void ParticleBatch2D::Update(float deltaTime) {
	for (int i = 0; i < p_maxParticles; i++) {
		if (p_particles[i].life <= 0.0f)
			continue;
		p_updateFunc(p_particles[i], deltaTime);
		p_particles[i].life -= p_decayRate * deltaTime;
	}
}

void ParticleBatch2D::Draw(SpriteBatch* spriteBatch) {
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < p_maxParticles; i++) {
		if (p_particles[i].life <= 0.0f)
			continue;
		auto& p = p_particles[i];
		glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
		spriteBatch->Draw(destRect, uvRect, p_texture.ID, 0.0f, p.color);
	}
}

void ParticleBatch2D::AddParticle(const glm::vec2 &position,
								  glm::vec2& velocity, 
								  ColorRGBA8 &color,
								  float width) {
	int particleIndex = FindFreeParticle();

	auto& p = p_particles[particleIndex];

	p.life = 1.0f;
	p.position = position;
	p.velocity = velocity;
	p.color = color;
	p.width = width;
}

int ParticleBatch2D::FindFreeParticle() {
	for (int i = p_lastFreeParticle; i < p_maxParticles; i++) {
		if (p_particles[i].life <= 0.0f) {
			p_lastFreeParticle = i;
			return i;
		}
	}
	for (int i = 0; i < p_lastFreeParticle; i++) {
		if (p_particles[i].life <= 0.0f) {
			p_lastFreeParticle = i;
			return i;
		}
	}

	//No particles are free, overwrite the last one
	return 0;
}