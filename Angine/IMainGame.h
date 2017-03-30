#pragma once

namespace Angine {

	class ScreenList;

	class IMainGame {
	protected:
		std::unique_ptr<ScreenList> p_screenList = nullptr;
		IGameScreen* p_curScreen = nullptr;
		bool p_isRunning = false;
		float p_fps = 0;
		Window p_window;

		bool Init(int screenWidth, int screenHeight, WindowFlags flags);
		bool InitSystems(int screenWidth, int screenHeight, WindowFlags flags);

		
		
	public:
		IMainGame();
		virtual ~IMainGame();

		InputManager inputManager;

		void Run(int screenWidth, int screenHeight, WindowFlags flags);
		void Exit();

		void OnSDLEvent(SDL_Event& evnt);
	
		virtual void OnInit() = 0;
		virtual void AddScreens() = 0;
		virtual void OnExit() = 0;

		virtual void Update();
		virtual void Draw();

		const float GetFPS() const { return p_fps; };
	};

}