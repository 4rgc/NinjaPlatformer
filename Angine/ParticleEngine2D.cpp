#include "angine.h"

using namespace Angine;

ParticleEngine2D::ParticleEngine2D() {
	//empty
}


ParticleEngine2D::~ParticleEngine2D() {
	for (auto& b : p_batches) {
		delete b;
	}
}

void ParticleEngine2D::AddParticleBatch(ParticleBatch2D* particleBatch) {
	p_batches.push_back(particleBatch);
}

void ParticleEngine2D::Update(float deltaTime) {
	for (auto& b : p_batches) {
		b->Update(deltaTime);
	}
}

void ParticleEngine2D::Draw(SpriteBatch* spriteBatch) {
	for (auto& b : p_batches) {
		spriteBatch->Begin();
		b->Draw(spriteBatch);
		spriteBatch->End();
		spriteBatch->RenderBatch();
	}
}