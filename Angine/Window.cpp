#include "angine.h"

namespace Angine {


	int Window::Create(std::string windowName, int screenWidth, int screenHeight, unsigned int curFlags) {

		p_screenWidth = screenWidth;
		p_screenHeight = screenHeight;

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (curFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;
		if (curFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (curFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		p_SDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (p_SDLWindow == nullptr) {
			std::string error = "SDL_CreateWindow failed: ";
				throwError(error);
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(p_SDLWindow);
		if (p_SDLWindow == nullptr) {
			std::string error = "SDL_GL_CreateContext failed: ";
				throwError(error);
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) {
			throwError("Glew didn't init");
		}

		//check the opengl version
		printf("*** OPEN GL VERSION: %s ***\n", glGetString(GL_VERSION));

		glClearColor(1.0f, 1.0f, 1.0f, 1.0);

		//VSYNC
		SDL_GL_SetSwapInterval(0);

		//enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::SwapBuffer() {
		SDL_GL_SwapWindow(p_SDLWindow);
	}

}