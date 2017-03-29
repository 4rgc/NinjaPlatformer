#pragma once
namespace Angine {

	class SoundEffect {
		friend class AudioEngine;
		Mix_Chunk* p_chunk = nullptr;
	public:
		void Play(int loops = 0);
	};

	class Music {
		friend class AudioEngine;
		Mix_Music* p_music = nullptr;
	public:
		void Play(int loops = -1);
		void Pause();
		void Stop();
		void Resume();
	};

	class AudioEngine
	{
		std::map<std::string, Mix_Chunk*> p_effectsMap;
		std::map<std::string, Mix_Music*> p_musicMap;

		bool p_isInit;
	public:
		AudioEngine();
		~AudioEngine();

		void Init();
		void Destroy();

		SoundEffect LoadSoundEffect(const std::string& filePath);
		Music LoadMusic(const std::string& filePath);
	};

}