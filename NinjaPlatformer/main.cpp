#include "Header.h"

int main(int argc, char** argv) {
	int w, h;
	Angine::WindowFlags runFlags;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "fullscreen")) {
			runFlags = Angine::WindowFlags::FULLSCREEN;
		}
		if (!strcmp(argv[i], "borderless")) {
			runFlags = Angine::WindowFlags::BORDERLESS;
		}
		if (!strcmp(argv[i], "w:")) {
			w = atoi(argv[i + 1]);
		}
		if (!strcmp(argv[i], "h:")) {
			h = atoi(argv[i + 1]);
		}
	}
	if (w == 0 || h == 0) {
		w = 1024;
		h = 768;
	}
	App app;
	app.Run(w, h, runFlags);
	return 0;
}