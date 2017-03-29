#pragma once

namespace Angine {

	class Particle2D {
	public:
		friend class ParticleBatch2D;
		glm::vec2 position = glm::vec2(0.0);
		glm::vec2 velocity;
		ColorRGBA8 color;
		float width = 0.0f;
		float life = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
	}

	class ParticleBatch2D {
		float p_decayRate = 0.1f;
		Particle2D* p_particles = nullptr;
		int p_maxParticles = 0;
		int p_lastFreeParticle = 0;
		GLTexture p_texture;
		std::function<void(Particle2D&, float)> p_updateFunc;
		int FindFreeParticle();
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void Init(int maxParticles, 
				  float decayRate, 
				  GLTexture& texture,
				  std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);

		void Update(float deltaTime);

		void Draw(SpriteBatch* spriteBatch);

		void AddParticle(const glm::vec2 &position,
						glm::vec2& velocity,
						ColorRGBA8& color,
						float width);
	};

}