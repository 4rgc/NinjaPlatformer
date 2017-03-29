#include "angine.h"

namespace Angine {

	void throwError(std::string error) {
		std::cout << error.c_str();
		system("Pause");
		SDL_Quit();
		exit(-1);
	}

}