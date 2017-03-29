#pragma once

namespace Angine {

	class ScreenList {
	protected:
		std::vector<IGameScreen*> p_screens;
		int p_curScreenIndex = -1;
		IMainGame* p_game = nullptr;
	public:
		ScreenList(IMainGame* game);
		~ScreenList();
		
		IGameScreen* MoveNext();
		IGameScreen* MovePrev();

		IGameScreen* GetCur();

		void SetScreen(int nextScreen);
		void AddScreen(IGameScreen* newScreen);

		void Destroy();
	};

}