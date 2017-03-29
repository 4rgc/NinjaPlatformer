#include "angine.h"

namespace Angine {

	FPSLimiter::FPSLimiter() {
	
	}

	void FPSLimiter::SetMaxFPS(float maxFPS) {
		p_maxFPS = maxFPS;
	}

	void FPSLimiter::Init(float maxFPS) {
		SetMaxFPS(maxFPS);
	}
	void FPSLimiter::Begin() {
		p_startTicks = SDL_GetTicks();
	}

	float FPSLimiter::End() {
		CalculateFPS();
		//limit fps
		float frameTicks = (float)(SDL_GetTicks() - p_startTicks);
		if (1000.0f / p_maxFPS > frameTicks) 
			SDL_Delay((Uint32)(1000.0f / p_maxFPS - frameTicks));
		return p_FPS;
	}

	void FPSLimiter::CalculateFPS() {
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int curFrame = 0;

		static float prevTicks = (float)SDL_GetTicks();
		float curTicks;
		curTicks = (float)SDL_GetTicks();
		p_frameTime = (float)(curTicks - prevTicks);
		frameTimes[curFrame%NUM_SAMPLES] = (float)p_frameTime;

		prevTicks = curTicks;

		int count;
		curFrame++;
		if (curFrame < NUM_SAMPLES) {
			count = curFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		float frameTimeAvg = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAvg += frameTimes[i];
		}

		frameTimeAvg /= count;

		if (frameTimeAvg > 0) {
			p_FPS = 1000.0f / frameTimeAvg;
		}
		else {
			p_FPS = 60.0f;
		}
	}
}