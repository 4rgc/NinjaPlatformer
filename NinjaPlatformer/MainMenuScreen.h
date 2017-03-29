#pragma once
class MainMenuScreen : public Angine::IGameScreen{

	Angine::Camera2D p_MainCamera;
	Angine::Window* p_window;
	Angine::GUI p_GUI;

	int p_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;

	void InitUI();
	bool OnExitClicked(const CEGUI::EventArgs& e);
	bool OnNewGameClicked(const CEGUI::EventArgs& e);
	bool OnEditorClicked(const CEGUI::EventArgs& e);

	void CheckInput();
public:
	MainMenuScreen(Angine::Window* window);
	~MainMenuScreen();

	int GetNextScreenIndex() const override;
	int GetPrevScreenIndex() const override;

	void Build() override;
	void Destroy() override;

	void OnEntry() override;
	void OnExit() override;

	void Update() override;
	void Draw() override;

};

