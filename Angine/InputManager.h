#pragma once

namespace Angine {

	class InputManager {
		std::unordered_map<unsigned int, bool> p_prevKeyMap;
		std::unordered_map<unsigned int, bool> p_keyMap;
		glm::vec2 p_mouseCoords;
		bool WasKeyDown(unsigned int keyID);
	public:
		InputManager();

		void Update();

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);
		void SetMouseCoords(float x, float y);
		bool IsKeyDown(unsigned int keyID);		///< returns true if the key is held
		bool IsKeyPressed(unsigned int keyID);	///< returns true if the key was just pressed

		glm::vec2 GetMouseCoords() const { return p_mouseCoords; }
	};

}