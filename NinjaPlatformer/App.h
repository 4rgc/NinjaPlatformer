#pragma once

class App : public Angine::IMainGame {

	std::unique_ptr<GameplayScreen> p_gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> p_mainmenuScreen = nullptr;
	std::unique_ptr<EditorScreen> p_editorScreen = nullptr;
public:
	App();
	~App();

	//Called on initialization
	virtual void OnInit() override;
	//For adding all the screens
	virtual void AddScreens() override;
	//Called when exiting
	virtual void OnExit() override;

};

