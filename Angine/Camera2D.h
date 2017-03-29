#pragma once

namespace Angine {

	class Camera2D {
		bool p_needsMatrixUpdate;
		glm::vec2 position;
		glm::mat4 p_cameraMatrix;
		glm::mat4 p_orthoMatrix;
		float p_scale;
		int p_screenWidth;
		int p_screenHeight;
	public:
		Camera2D();
		void Update();

		bool IsBoxInView(const glm::vec2& position, const glm::vec2 &dimensions);

		glm::vec2 &ConvertScreenToWorld(glm::vec2& screenCoords);

		void Init(int screenW, int screenH);
		//set&get
		void SetPosition(glm::vec2 &newPosition) { position = newPosition; 
		p_needsMatrixUpdate= 1;}
		glm::vec2 GetPosition() { return position; }
		void SetScale(float newScale) { p_scale = newScale; p_needsMatrixUpdate = 1; }
		float GetScale() { return p_scale; }
		glm::mat4 GetMatrix() { return p_cameraMatrix; }
	};

}