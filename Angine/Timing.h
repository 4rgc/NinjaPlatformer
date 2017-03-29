#pragma once

namespace Angine {

	class FPSLimiter
	{
		float p_FPS;
		float p_maxFPS;
		int p_frameTime;
		unsigned int p_startTicks;

		void CalculateFPS();
	public:
		FPSLimiter();
		void SetMaxFPS(float maxFPS);
		void Init(float maxFPS);
		void Begin();
		//returns fps
		float End();
	};
}