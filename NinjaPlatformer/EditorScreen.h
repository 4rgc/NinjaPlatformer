#pragma once

class EditorScreen : public Angine::IGameScreen {

	Angine::Camera2D p_MainCamera;
	Angine::Window* p_window;
	Angine::GUI p_GUI;
	Angine::SpriteBatch p_spriteBatch;
	Angine::GLSLProgram p_textureProgram;
	Angine::SpriteFont p_spriteFont;

	GLuint p_eTex = Angine::ResourceManager::GetTexture("Textures/empty.png").ID;

	void InitUI();
	bool OnExitClicked(const CEGUI::EventArgs& e);

	glm::vec3 p_colorPicker = glm::vec3(255.0f);

	CEGUI::Slider* p_rSlider = nullptr;
	CEGUI::Slider* p_gSlider = nullptr;
	CEGUI::Slider* p_bSlider = nullptr;

	bool OnColorPickerRed(const CEGUI::EventArgs& e);
	bool OnColorPickerGreen(const CEGUI::EventArgs& e);
	bool OnColorPickerBlue(const CEGUI::EventArgs& e);

	void CheckInput();
public:
	EditorScreen(Angine::Window* window);
	~EditorScreen();

	int GetNextScreenIndex() const override;
	int GetPrevScreenIndex() const override;

	void Build() override;
	void Destroy() override;

	void OnEntry() override;
	void OnExit() override;

	void Update() override;
	void Draw() override;

};

