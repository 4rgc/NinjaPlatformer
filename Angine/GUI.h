#pragma once

namespace Angine {

	class GUI {
		static CEGUI::OpenGL3Renderer* p_renderer;
		CEGUI::GUIContext* p_context = nullptr;
		CEGUI::Window* p_root;
		unsigned int p_lastTime = 0;
	public:
		void Init(const std::string& resourceDir);
		void Destroy();
		 
		static CEGUI::OpenGL3Renderer* getRenderer() { return p_renderer; }
		const CEGUI::GUIContext* getContext() const { return p_context; }
		CEGUI::Window* CreateWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
		static void SetWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);
		void SetMouseCursor(const std::string& imageFile);
		void ShowMouswCursor();
		void HideMouseCursor();

		void OnSDLEvent(SDL_Event& evnt);

		void Draw();
		void Update();

		void LoadScheme(const std::string& schemeFile);
		void SetFont(const std::string& fontFile);
	};

}