#pragma once

class GameplayScreen : public Angine::IGameScreen {

	std::unique_ptr<b2World> p_world;
	std::vector<Box> p_boxes;

	Angine::DebugRenderer p_debugRenderer;
	Angine::SpriteBatch* p_spriteBatch;
	Angine::SpriteBatch p_HUDSpriteBatch;
	Angine::SpriteFont* p_spriteFont;
	Angine::GLSLProgram p_textureProgram;
	Angine::GLSLProgram p_lightProgram;
	Angine::Camera2D p_MainCamera;
	Angine::Window* p_window;
	Angine::GUI p_GUI;
	
	std::vector<CEGUI::Window*> p_updGUI;

	std::vector<std::string> p_levelPaths;

	Player* p_player; ///< const reference

	Level p_curLvl;
	int p_levelIndex;

	void InitUI();
	bool OnExitClicked(const CEGUI::EventArgs& e);

	void CheckInput();

	void DrawWin();
	void DrawLose();
public:
	GameplayScreen(Angine::Window* window);
	~GameplayScreen();

	int GetNextScreenIndex() const override;
	int GetPrevScreenIndex() const override;

	void Build() override;
	void Destroy() override;

	void OnEntry() override;
	void OnExit() override;

	void Update() override;
	void Draw() override;

};

