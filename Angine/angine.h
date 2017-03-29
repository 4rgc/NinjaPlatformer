#pragma once
#include <Windows.h>

#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <fstream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <TTF/SDL_ttf.h>
#include <SDL2/SDL_mixer.h> 
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <memory>
#include "picopng.h"

#include "Window.h"

namespace Angine {
	class Camera2D;
	extern void throwError(std::string);
	extern int Init();
}
#include "GLTexture.h"
#include "TextureCache.h"
#include "ResourceManager.h"
#include "Vertex.h"
#include "SpriteFont.h"
#include "IOManager.h"
#include "ImageLoader.h"
#include "GLSLProgram.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "TileSheet.h"
#include "Camera2D.h"
#include "InputManager.h"
#include "Timing.h"
#include "ParticleBatch2D.h"
#include "ParticleEngine2D.h"
#include "AudioEngine.h"
#include "GUI.h"
#include "IGameScreen.h"
#include "IMainGame.h"
#include "ScreenList.h"
#include "DebugRenderer.h"