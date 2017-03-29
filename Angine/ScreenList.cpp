#include "angine.h"

using namespace Angine;

ScreenList::ScreenList(IMainGame* game) :
	p_game(game)
			{}

ScreenList::~ScreenList() {
	Destroy();
}

IGameScreen* ScreenList::MoveNext() {
	IGameScreen* curScreen = GetCur();
	if (curScreen->GetNextScreenIndex() != SCREEN_INDEX_NO_SCREEN)
		p_curScreenIndex = curScreen->GetNextScreenIndex();
	return GetCur();
}

IGameScreen* ScreenList::MovePrev() {
	IGameScreen* curScreen = GetCur();
	if (curScreen->GetPrevScreenIndex() != SCREEN_INDEX_NO_SCREEN)
		p_curScreenIndex = curScreen->GetPrevScreenIndex();
	return GetCur();
}

IGameScreen* ScreenList::GetCur() {
	if (p_curScreenIndex == SCREEN_INDEX_NO_SCREEN)
		return nullptr;
	return p_screens[p_curScreenIndex];
}

void ScreenList::SetScreen(int nextScreen) {
	p_curScreenIndex = nextScreen;
}

void ScreenList::AddScreen(IGameScreen* newScreen) {
	newScreen->p_screenIndex = p_screens.size();
	p_screens.push_back(newScreen);
	newScreen->Build();
	newScreen->SetParGame(p_game);
}

void ScreenList::Destroy() {
	for (size_t i = 0; i < p_screens.size(); i++) {
		p_screens[i]->Destroy();
	}
	p_screens.resize(0);
	p_curScreenIndex = SCREEN_INDEX_NO_SCREEN;
}
