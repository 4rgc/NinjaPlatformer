#include "Header.h"



MainMenuScreen::MainMenuScreen(Angine::Window* window) : 
				p_window(window) {
	p_screenIndex = SCREEN_INDEX_MAINMENU;

	p_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	p_textureProgram.AddAttribute("vertexPosition");
	p_textureProgram.AddAttribute("vertexColor");
	p_textureProgram.AddAttribute("vertexUV");
	p_textureProgram.LinkShaders();

	p_spriteBatch.Init();
	p_HUDSpriteBatch.Init();

	p_spriteFont = new Angine::SpriteFont("Fonts/BalooBhaina.ttf", 64);
}

MainMenuScreen::~MainMenuScreen() {

}

void MainMenuScreen::InitUI() {
	p_GUI.Init("GUI");
	p_GUI.LoadScheme("TaharezLook.scheme");
	p_GUI.SetFont("DejaVuSans-10");

	CEGUI::PushButton* startButton = static_cast<CEGUI::PushButton*>(p_GUI.CreateWidget("TaharezLook/Button", glm::vec4(0.5f - 0.075f, 0.50f, 0.15f, 0.06f), glm::vec4(0.0f), "StartButton"));
	startButton->setText("Start game");
	//Set the event to be calles when we click
	startButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::OnNewGameClicked, this));

	CEGUI::PushButton* editorButton = static_cast<CEGUI::PushButton*>(p_GUI.CreateWidget("TaharezLook/Button", glm::vec4(0.5f - 0.075f, 0.60f, 0.15f, 0.06f), glm::vec4(0.0f), "EditButton"));
	editorButton->setText("Level Editor");
	//Set the event to be calles when we click
	editorButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::OnEditorClicked, this));
	editorButton->disable();

	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(p_GUI.CreateWidget("TaharezLook/Button", glm::vec4(0.5f - 0.075f, 0.7f, 0.15f, 0.06f), glm::vec4(0.0f), "ExitButton"));
	exitButton->setText("Exit game");
	//Set the event to be called when we click
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::OnExitClicked, this));

	p_GUI.SetMouseCursor("TaharezLook/MouseArrow");
	p_GUI.ShowMouseCursor();
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
	p_textureProgram.Dispose();
}

void MainMenuScreen::OnEntry() {


	p_MainCamera.Init(p_window->getScreenW(), p_window->getScreenH());
	p_MainCamera.SetScale(50.0f);

	p_xOffset = -((p_window->getScreenW() / 2.0f) / p_MainCamera.GetScale());
	p_yOffset = -((p_window->getScreenH() / 2.0f) / p_MainCamera.GetScale());
	
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

	p_textureProgram.Use();

	//Upload texture Uniforms
	GLint textureUniform = p_textureProgram.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = p_MainCamera.GetMatrix();
	GLint pUniform = p_textureProgram.GetUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	p_spriteBatch.Begin();

	p_spriteBatch.Draw(glm::vec4(p_MainCamera.GetPosition().x + p_xOffset, p_MainCamera.GetPosition().y + p_yOffset, -p_xOffset * 2.0f, -p_yOffset * 2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), Angine::ResourceManager::GetTexture("Textures/textures/bg.png").ID, 0.0f, Angine::ColorRGBA8(255, 255, 255, 255));

	p_spriteBatch.End();
	p_spriteBatch.RenderBatch();

	p_HUDSpriteBatch.Begin();

	p_spriteFont->draw(p_HUDSpriteBatch, "Ninja Platformer", glm::vec2(p_MainCamera.GetPosition().x, p_MainCamera.GetPosition().y + 1.5f), glm::vec2(0.04f, 0.03f), 0.0f, Angine::ColorRGBA8(0, 0, 180, 255), Angine::Justification::MIDDLE);

	p_HUDSpriteBatch.End();
	p_HUDSpriteBatch.RenderBatch();

	p_textureProgram.UnUse();

	p_GUI.Draw();
	glEnable(GL_BLEND);
}
