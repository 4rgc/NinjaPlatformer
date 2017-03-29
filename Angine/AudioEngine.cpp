#include "angine.h"

using namespace Angine;

void SoundEffect::Play(int loops) {
	if (Mix_PlayChannel(-1, p_chunk, loops) == -1) {
		if(Mix_PlayChannel(0, p_chunk, loops) == -1)
			throwError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
	}
}

void Music::Play(int loops) {
	if (Mix_PlayMusic(p_music, loops) == -1) {
		throwError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
	}
}

void Music::Pause() {
	Mix_PauseMusic();
}

void Music::Stop() {
	Mix_HaltMusic();
}
void Music::Resume() {
	Mix_ResumeMusic();
}


AudioEngine::AudioEngine() {
	//empty
}


AudioEngine::~AudioEngine() {
	Destroy();
}

void AudioEngine::Init() {
	if (p_isInit)
		throwError("Tried to init engine twice");
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG)) {
		throwError("Mix_Init failed: " + std::string(Mix_GetError()));
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throwError("Mix_OpenAudio failed: " + std::string(Mix_GetError()));
	}

	p_isInit = 1;
}

void AudioEngine::Destroy() {
	if (p_isInit) {
		p_isInit = 0;

		for (auto& it : p_effectsMap)
			Mix_FreeChunk(it.second);

		for (auto& it : p_musicMap)
			Mix_FreeMusic(it.second);

		p_effectsMap.clear();
		p_musicMap.clear();

		Mix_CloseAudio();
		Mix_Quit();
	}
}

SoundEffect AudioEngine::LoadSoundEffect(const std::string& filePath) {
	//try to find audio in the cache
	auto it = p_effectsMap.find(filePath);

	SoundEffect effect;

	if (it == p_effectsMap.end()) {
		//failed to find, must load from disk
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
		if (chunk == nullptr) {
			throwError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
		}

		p_effectsMap[filePath] = chunk;
		effect.p_chunk = chunk;
	}
	else {
		//loads from cache
		effect.p_chunk = it->second;
	}

	return effect;
}

Music AudioEngine::LoadMusic(const std::string& filePath) {
	//try to find audio in the cache
	auto it = p_musicMap.find(filePath);

	Music music;

	if (it == p_musicMap.end()) {
		//failed to find, must load from disk
		Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
		if (mixMusic == nullptr) {
			throwError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
		}

		p_musicMap[filePath] = mixMusic;
		music.p_music = mixMusic;
	}
	else {
		//loads from cache
		music.p_music = it->second;
	}

	return music;
}
