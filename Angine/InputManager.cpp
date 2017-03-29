#include "angine.h"

using namespace Angine;

	InputManager::InputManager() :
		p_mouseCoords(0)
		{}

	void InputManager::PressKey(unsigned int keyID) {
		p_keyMap[keyID] = true;
	}

	void InputManager::ReleaseKey(unsigned int keyID) {
		p_keyMap[keyID] = false;
	}

	bool InputManager::IsKeyDown(unsigned int keyID) {
		auto it = p_keyMap.find(keyID);
		if (it != p_keyMap.end())
			return it->second;
		else
			return false;

	}

	void InputManager::SetMouseCoords(float x, float y) {
		p_mouseCoords.x = x;
		p_mouseCoords.y = y;
	
	}

	void InputManager::Update() {
		//loop through p_keyMap using a foreach loop, and copy values to p_prevKeyMap
		for (auto &it : p_keyMap)
			p_prevKeyMap[it.first] = it.second;
	}

	bool InputManager::IsKeyPressed(unsigned int keyID) {
		if (IsKeyDown(keyID) == true  && WasKeyDown(keyID)==false)
			return true;
		return false;
	}

	bool InputManager::WasKeyDown(unsigned int keyID) {
		auto it = p_prevKeyMap.find(keyID);
		if (it != p_prevKeyMap.end())
			return it->second;
		else
			return false;

	}