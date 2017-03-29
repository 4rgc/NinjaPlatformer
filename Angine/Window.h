#pragma once

namespace Angine {

	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window {
		SDL_Window* p_SDLWindow;
		int p_screenWidth, p_screenHeight;
	public:

		inline int getScreenW() const { return p_screenWidth; }
		inline int getScreenH() const { return p_screenHeight; }

		int Create(std::string windowName, int screenWidth, int screenHeight, unsigned int curFlags);
		void SwapBuffer();
	};

}