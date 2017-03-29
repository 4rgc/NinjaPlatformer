#pragma once
namespace Angine {
	class ParticleEngine2D {
		std::vector<ParticleBatch2D*> p_batches;
	public:
		ParticleEngine2D();
		~ParticleEngine2D();
	
		//after adding a batch this class becomes responsible for deallocation
		void AddParticleBatch(ParticleBatch2D* particleBatch);
		void Update(float deltaTime);
		void Draw(SpriteBatch* spriteBatch);
	};

}