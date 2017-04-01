#include "Header.h"
#include <stdarg.h>

#define NO_DEBUG_RENDER

GameplayScreen::GameplayScreen(Angine::Window* window) :
	p_window(window)
				  {
	p_screenIndex = SCREEN_INDEX_GAMEPLAY;	
	//Initialize the spritebatch
	p_spriteBatch = new Angine::SpriteBatch();
	p_spriteBatch->Init();

	//Shaders Init
	//Compile texture shaders
	p_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	p_textureProgram.AddAttribute("vertexPosition");
	p_textureProgram.AddAttribute("vertexColor");
	p_textureProgram.AddAttribute("vertexUV");
	p_textureProgram.LinkShaders();
	//Compile light shaders
	p_lightProgram.CompileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	p_lightProgram.AddAttribute("vertexPosition");
	p_lightProgram.AddAttribute("vertexColor");
	p_lightProgram.AddAttribute("vertexUV");
	p_lightProgram.LinkShaders();

	//Init the debugRenderer
	p_debugRenderer.Init();
}


GameplayScreen::~GameplayScreen()
{
	b2World* del = p_world.release();
	delete del;
}

int GameplayScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_MAINMENU;
}

void GameplayScreen::Build() {

}

void GameplayScreen::Destroy() {
	p_textureProgram.Dispose();
	p_lightProgram.Dispose();
	p_debugRenderer.Dispose();
}

void GameplayScreen::OnEntry() {
	b2Vec2 gravity(0.0f, -30.0f);
	p_world = std::make_unique<b2World>(gravity);

	std::mt19937 randEngine(time(NULL));
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);
	std::uniform_int_distribution<int> color(0, 120);

	std::uniform_real_distribution<float> size(0.5f, 2.5f);

	const int NUM_BOXES = 0;

	//Load the textures
	Angine::GLTexture boxTexture = Angine::ResourceManager::GetTexture("Textures/red_bricks.png");

	for (int i = 0; i < NUM_BOXES; i++) {
		Box newBox;
		float sizee = size(randEngine);
		BoxDef def(p_world.get(), b2Vec2(xPos(randEngine), yPos(randEngine)), b2Vec2(sizee, sizee));
		def.color = Angine::ColorRGBA8(color(randEngine), color(randEngine), color(randEngine), (GLubyte)255);
		newBox.Init(&def, false, false, boxTexture);
		p_boxes.push_back(newBox);
	}

	//Init camera
	p_MainCamera.Init(p_window->getScreenW(), p_window->getScreenH());
	p_MainCamera.SetScale(52.0f);


	//Init the level and player
	p_curLvl = Level(p_world.get(), p_window, p_MainCamera);
	p_curLvl.Load("empty.txt");
	p_curLvl.Init();
	p_player = const_cast<Player*>(p_curLvl.GetPlayerP());

	//Init the UI
	InitUI();

}

void GameplayScreen::InitUI() {
	p_GUI.Init("GUI");
	p_GUI.LoadScheme("TaharezLook.scheme");
	p_GUI.SetFont("DejaVuSans-10");
	//Exit button
	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(p_GUI.CreateWidget("TaharezLook/Button", glm::vec4(0.05f, 0.05f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
	exitButton->setText("Exit Game");
	//Enemies left label
	CEGUI::DefaultWindow* leftLabel = static_cast<CEGUI::DefaultWindow*>(p_GUI.CreateWidget("TaharezLook/Label", glm::vec4(0.2f, 0.05f, 0.3f, 0.05f), glm::vec4(0.0f), "EnemiesLeftLabel"));
	p_updGUI.push_back(leftLabel);
	//HP left label
	CEGUI::DefaultWindow* HPLabel = static_cast<CEGUI::DefaultWindow*>(p_GUI.CreateWidget("TaharezLook/Label", glm::vec4(0.4f, 0.05f, 0.3f, 0.05f), glm::vec4(0.0f), "HPLabel"));
	p_updGUI.push_back(HPLabel);
	//Lambda for level stat updating
	p_GUI.CustomUpdate = [windows = p_updGUI, &level = p_curLvl]() ->void{
		windows[0]->setText("Enemies left: " + std::to_string(level.GetEnemiesLeft()));
		windows[1]->setText("HP: " + std::to_string(level.GetPlayerP()->GetHP()));
	};

	//Set the event to be called when we click
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::OnExitClicked, this));

	p_GUI.SetMouseCursor("TaharezLook/MouseArrow");
	p_GUI.ShowMouseCursor();
	SDL_ShowCursor(0);
}

bool GameplayScreen::OnExitClicked(const CEGUI::EventArgs& e) {
	std::cout << "quit" << "\n";
	p_curState = Angine::ScreenState::EXIT_APPLICATION;
	return 1;
}

void GameplayScreen::OnExit() {
	p_curLvl.Destroy();
	p_GUI.Destroy();
	p_updGUI.clear();
}

void GameplayScreen::Update() {
	p_MainCamera.SetPosition(glm::vec2(p_player->GetPosition().x + p_window->getScreenW() /4.5f / p_MainCamera.GetScale(), p_player->GetPosition().y));
	if (p_MainCamera.GetPosition().x < 0.0f)
		p_MainCamera.SetPosition(glm::vec2(0.0f, p_MainCamera.GetPosition().y));
	if (p_MainCamera.GetPosition().y < 0.0f)
		p_MainCamera.SetPosition(glm::vec2(p_MainCamera.GetPosition().x, 0.0f));
	p_MainCamera.Update();
	CheckInput();
	p_GUI.Update();
	if (p_curLvl.Update(p_game->inputManager)) {
		p_curState = Angine::ScreenState::CHANGE_PREVIOUS;
		p_player = nullptr;
		return;
	}
	//Update the physics simulation
	p_world->Step(1.0f / 120.0f, 6, 2);
}

void GameplayScreen::Draw() {
	if (!p_player) {
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.15f, 1.0f);

	p_textureProgram.Use();

	//Upload texture Uniforms
	GLint textureUniform = p_textureProgram.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = p_MainCamera.GetMatrix();
	GLint pUniform = p_textureProgram.GetUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	p_spriteBatch->Begin();

	p_curLvl.Draw(p_MainCamera, *p_spriteBatch);

	p_spriteBatch->End();
	p_spriteBatch->RenderBatch();

	p_textureProgram.UnUse();

	//Additive blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//Render some test lights
	Light playerLight;
	playerLight.color = Angine::ColorRGBA8(255, 255, 255, 30);
	playerLight.position = p_player->GetPosition();
	playerLight.size = 20.0f;

	p_lightProgram.Use();
	pUniform = p_lightProgram.GetUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	p_spriteBatch->Begin();

	playerLight.Draw(*p_spriteBatch);
	//mouseLight.Draw(p_spriteBatch);

	p_spriteBatch->End();
	p_spriteBatch->RenderBatch();

	p_lightProgram.UnUse();


	//Reset to regular alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Debug rendering
#ifdef DEBUG_RENDER
	glm::vec4 destRect;
	for (auto& b : p_boxes) {
		destRect.x = b.GetBody()->GetPosition().x - b.GetDims().x / 2.0f;
		destRect.y = b.GetBody()->GetPosition().y - b.GetDims().y / 2.0f;
		destRect.w = b.GetDims().x;
		destRect.z = b.GetDims().y;
		p_debugRenderer.DrawBox(destRect, Angine::ColorRGBA8(255,255,255,255), b.GetBody()->GetAngle());
	}
	p_curLvl.DrawDebug(p_debugRenderer);
	p_debugRenderer.End();
	p_debugRenderer.Render(projectionMatrix, 2.0f);
#endif
	
	p_GUI.Draw();
	glEnable(GL_BLEND);
}

void GameplayScreen::CheckInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		p_game->OnSDLEvent(evnt);
		p_GUI.OnSDLEvent(evnt);
	}
}