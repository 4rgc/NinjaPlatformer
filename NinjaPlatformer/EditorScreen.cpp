#include "Header.h"



EditorScreen::EditorScreen(Angine::Window* window) :
							p_window(window),
							p_spriteFont("Fonts/font.ttf", 32)
						{
	p_screenIndex = SCREEN_INDEX_EDITOR;
}

EditorScreen::~EditorScreen() {

}

void EditorScreen::InitUI() {
	p_GUI.Init("GUI");
	p_GUI.LoadScheme("TaharezLook.scheme");
	p_GUI.SetFont("DejaVuSans-10");
	
	{   //Add the color slider
		const float X_DIM = 0.02f, Y_DIM = 0.15f;
		const float Y_POS = 0.05f;
		const float PADDING = 0.01f;

		p_rSlider = static_cast<CEGUI::Slider*>(p_GUI.CreateWidget("TaharezLook/Slider", glm::vec4(0.05f, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "rSlider"));
		p_rSlider->setMaxValue(255.0f);
		p_rSlider->setCurrentValue(p_colorPicker.r);
		p_rSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::OnColorPickerRed, this));
		p_rSlider->setClickStep(1.0f);

		p_gSlider = static_cast<CEGUI::Slider*>(p_GUI.CreateWidget("TaharezLook/Slider", glm::vec4(0.05f + X_DIM + PADDING, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "gSlider"));
		p_gSlider->setMaxValue(255.0f);
		p_gSlider->setCurrentValue(p_colorPicker.g);
		p_gSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::OnColorPickerGreen, this));
		p_gSlider->setClickStep(1.0f);

		p_bSlider = static_cast<CEGUI::Slider*>(p_GUI.CreateWidget("TaharezLook/Slider", glm::vec4(0.05f + (X_DIM + PADDING)*2, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "bSlider"));
		p_bSlider->setMaxValue(255.0f);
		p_bSlider->setCurrentValue(p_colorPicker.b);
		p_bSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::OnColorPickerBlue, this));
		p_bSlider->setClickStep(1.0f);
	}

	p_GUI.SetMouseCursor("TaharezLook/MouseArrow");
	p_GUI.ShowMouswCursor();
	SDL_ShowCursor(0);
}

bool EditorScreen::OnExitClicked(const CEGUI::EventArgs & e) {
	p_curState = Angine::ScreenState::EXIT_APPLICATION;
	return true;
}

bool EditorScreen::OnColorPickerRed(const CEGUI::EventArgs& e) {
	p_colorPicker.r = p_rSlider->getCurrentValue();
	return true;
}

bool EditorScreen::OnColorPickerGreen(const CEGUI::EventArgs& e) {
	p_colorPicker.g = p_gSlider->getCurrentValue();
	return true;
}

bool EditorScreen::OnColorPickerBlue(const CEGUI::EventArgs& e) {
	p_colorPicker.b = p_bSlider->getCurrentValue(); 
	return true;
}

void EditorScreen::CheckInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		p_game->OnSDLEvent(evnt);
		p_GUI.OnSDLEvent(evnt);
	}
}

int EditorScreen::GetNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int EditorScreen::GetPrevScreenIndex() const {
	return SCREEN_INDEX_MAINMENU;
}

void EditorScreen::Build() {

}

void EditorScreen::Destroy() {

}

void EditorScreen::OnEntry() {
	p_MainCamera.Init(p_window->getScreenW(), p_window->getScreenH());
	p_MainCamera.SetScale(1.0f);

	InitUI();

	p_spriteBatch.Init();

	p_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	p_textureProgram.AddAttribute("vertexPosition");
	p_textureProgram.AddAttribute("vertexColor");
	p_textureProgram.AddAttribute("vertexUV");
	p_textureProgram.LinkShaders();
}

void EditorScreen::OnExit() {
	p_GUI.Destroy();
	p_textureProgram.Dispose();
}

void EditorScreen::Update() {
	p_MainCamera.Update();
	CheckInput();
}

void EditorScreen::Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	p_textureProgram.Use();

	//Upload texture Uniform
	GLint textureUniform = p_textureProgram.GetUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	//Camera matrix
	glm::mat4 projectionMatrix = p_MainCamera.GetMatrix();
	GLint pUniform = p_textureProgram.GetUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	{	//Preview the picked color
		const float QUAD_SIZE = 50.0f;
		p_spriteBatch.Begin();
		glm::vec4 destRect = glm::vec4(p_bSlider->getXPosition().d_scale * p_window->getScreenW() + QUAD_SIZE - p_window->getScreenW()/2.0f,
			p_bSlider->getYPosition().d_scale * p_window->getScreenH() + p_window->getScreenH()/2.0f - p_bSlider->getHeight().d_scale * p_window->getScreenH() * 1.5f,
			QUAD_SIZE,
			QUAD_SIZE);

		p_spriteBatch.Draw(destRect, glm::vec4(0.0f,0.0f, 1.0f, 1.0f), p_eTex, 0.0f, Angine::ColorRGBA8((GLubyte)p_colorPicker.r, (GLubyte)p_colorPicker.g, (GLubyte)p_colorPicker.b, (GLubyte)255));
		p_spriteBatch.End();
	}
	p_spriteBatch.RenderBatch();
	p_textureProgram.UnUse();
	p_GUI.Draw();
}

