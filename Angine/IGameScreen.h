#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace Angine {

	class IMainGame;

	enum class ScreenState
	{
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IGameScreen {
		friend class ScreenList;
	protected:
		ScreenState p_curState = ScreenState::NONE;
		IMainGame* p_game = nullptr;
		int p_screenIndex = -1;
	public:
		IGameScreen() {
			//empty
		}
		virtual ~IGameScreen() {
			//empty
		}

		//Returns the index of the next or previous screen when changing screens
		virtual int GetNextScreenIndex() const = 0;
		virtual int GetPrevScreenIndex() const = 0;

		//Called at the beginning and end of an app
		virtual void Build() = 0;
		virtual void Destroy() = 0;

		//Called when a screen enters and exits focus
		virtual void OnEntry() = 0;
		virtual void OnExit() = 0;

		//Called in the main game loop
		virtual void Update() = 0;
		virtual void Draw() = 0;

		int GetScreenIndex() const { return  p_screenIndex; }
		ScreenState GetScreenState() const { return p_curState; }
		void SetRunning() { p_curState = ScreenState::RUNNING; }
		void SetParGame(IMainGame* game) { p_game = game; }

	};

}