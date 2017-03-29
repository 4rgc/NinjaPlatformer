#include "Header.h"



App::App() {

}


App::~App() {

}

void App::OnInit() {

}

void App::AddScreens() {
	p_mainmenuScreen = std::make_unique<MainMenuScreen>(&p_window);
	p_gameplayScreen = std::make_unique<GameplayScreen>(&p_window);
	p_editorScreen = std::make_unique<EditorScreen>(&p_window);

	p_screenList->AddScreen(p_mainmenuScreen.get());
	p_screenList->AddScreen(p_gameplayScreen.get());
	p_screenList->AddScreen(p_editorScreen.get());
	p_screenList->SetScreen(p_mainmenuScreen->GetScreenIndex());
}

void App::OnExit() {

}