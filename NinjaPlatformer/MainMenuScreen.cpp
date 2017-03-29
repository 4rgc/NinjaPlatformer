#include "Header.h"



MainMenuScreen::MainMenuScreen(Angine::Window* window) : 
				p_window(window) {
	p_screenIndex = SCREEN_INDEX_MAINMENU;

}

MainMenuScreen::~MainMenuScreen() {

}

void MainMenuScreen::InitUI() {
	p_GUI.Init("GUI");
	p_GUI.LoadScheme("TaharezLook.scheme");
	p_GUI.SetFont("DejaVuSans-10");

	CEGUI::PushButton* startButton = static_cast<CEGUI::PushButton*>(p_GUI.CreateWidget("TaharezLook/Button", glm::vec4(0.5f - 0.075f, 0.40f, 0.15f, 0.06f), glm::vec4(0.0f), "StartButton"));
	startButton->setText("Start game");
	//Set the event to be calles when we click
	startButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::OnNewGameClicked, this));

	CEGUI::PushButton* editorButton = static_cast<CEGUI::PushButton*>(p_GUI.CreateWidget("TaharezLook/Button", glm::vec4(0.5f - 0.075f, 0.50f, 0.15f, 0.06f), glm::vec4(0.0f), "EditButton"));
	editorButton->setText("Level Editor");
	//Set the event to be calles when we click
	editorButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::OnEditorClicked, this));

	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(p_GUI.CreateWidget("TaharezLook/Button", glm::vec4(0.5f - 0.075f, 0.6f, 0.15f, 0.06f), glm::vec4(0.0f), "ExitButton"));
	exitButton->setText("Exit game");
	//Set the event to be called when we click
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::OnExitClicked, this));

	p_GUI.SetMouseCursor("TaharezLook/MouseArrow");
	p_GUI.ShowMouswCursor();
	SDL_ShowCursor(0);
}

bool MainMenuScreen::OnExitClicked(const CEGUI::EventArgs& e) {
	p_curState = Angine::ScreenState::EXIT_APPLICATION;
	return true;
}

bool MainMenuScreen::OnNewGameClicked(const CEGUI::EventArgs& e) {
	p_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
	p_curState = Angine::ScreenState::CHANGE_NEXT;
	return true;
}

bool MainMenuScreen::OnEditorClicked(const CEGUI::EventArgs & e) {
	p_nextScreenIndex = SCREEN_INDEX_EDITOR;
	p_curState = Angine::ScreenState::CHANGE_NEXT;
	return true;
}

void MainMenuScreen::CheckInput() {
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			p_game->OnSDLEvent(evnt);
			p_GUI.OnSDLEvent(evnt);
		}
}

int MainMenuScreen::GetNextScreenIndex() const {
	return p_nextScreenIndex;
}

int MainMenuScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_MAINMENU;
}

void MainMenuScreen::Build() {

}

void MainMenuScreen::Destroy() {

}

void MainMenuScreen::OnEntry() {

	p_MainCamera.Init(p_window->getScreenW(), p_window->getScreenH());
	p_MainCamera.SetScale(50.0f);
	
	InitUI();
}

void MainMenuScreen::OnExit() {
	p_GUI.Destroy();
}

void MainMenuScreen::Update() {
	CheckInput();
	p_MainCamera.Update();
}

void MainMenuScreen::Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	p_GUI.Draw();
	glEnable(GL_BLEND);
}
