#include "angine.h"

using namespace Angine;


IMainGame::IMainGame() {
	p_screenList = std::make_unique<ScreenList>(this);
}

IMainGame::~IMainGame() {
	//empty
}

void IMainGame::OnSDLEvent(SDL_Event& evnt) {
	switch (evnt.type) {
		case SDL_QUIT:
			Exit();
			break;
		case SDL_KEYDOWN:
			inputManager.PressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.ReleaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.PressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.ReleaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			inputManager.SetMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
	}
}

bool IMainGame::Init() {
	Angine::Init();
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
	if(!InitSystems()) return false;
	OnInit();
	AddScreens();
	p_curScreen = p_screenList->GetCur();
	p_curScreen->OnEntry();
	p_curScreen->SetRunning();
	return true;
}

bool IMainGame::InitSystems() {
	p_window.Create("default_name", 1376, 768, 0);
	return true;
}

void IMainGame::Run() {
	
	if (!Init()) return;

	p_isRunning = 1;
	FPSLimiter limiter;
	limiter.SetMaxFPS(120.0f);
	while (p_isRunning) {
		limiter.Begin();

		inputManager.Update();
		
		Update();
		if (p_isRunning) {
			Draw();

			p_fps = limiter.End();
		}
	}
}

void IMainGame::Update() {
	if (p_curScreen) {
		switch (p_curScreen->GetScreenState()) {
			case ScreenState::RUNNING:
				p_curScreen->Update();
				break;
			case ScreenState::CHANGE_NEXT:
				p_curScreen->OnExit();
				p_curScreen = p_screenList->MoveNext();
				if (p_curScreen) {
					p_curScreen->SetRunning();
					p_curScreen->OnEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				p_curScreen->OnExit();
				p_curScreen = p_screenList->MovePrev();
				if (p_curScreen) {
					p_curScreen->SetRunning();
					p_curScreen->OnEntry();
				}
				break;
			case ScreenState::EXIT_APPLICATION:
				Exit();
				break;
		default:

			break;
		}
	}
	else {
		Exit();
	}
}

void IMainGame::Draw() {
	glViewport(0,0,p_window.getScreenW(), p_window.getScreenH());
	if (p_curScreen && p_curScreen->GetScreenState() == ScreenState::RUNNING) {
		p_curScreen->Draw();
	}
	p_window.SwapBuffer();
}

void IMainGame::Exit() {
	p_curScreen->OnExit();
	if (p_screenList) {
		p_screenList->Destroy();
		p_screenList.reset();
	}
	p_isRunning = false;
}
