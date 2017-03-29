#include "angine.h"

namespace Angine {
	Camera2D::Camera2D() :
		position((0.0f, 0.0f)),
		p_scale(1.0f),
		p_needsMatrixUpdate(true),
		p_screenWidth(500),
		p_screenHeight(500)
	{
		//p_cameraMatrix = glm::mat4{ 1.0f,1.0f,1.0f,1.0f,
		//						1.0f,1.0f,1.0f,1.0f,
		//				 		1.0f,1.0f,1.0f,1.0f,
		//					 	1.0f,1.0f,1.0f,1.0f};
		//p_orthoMatrix = glm::mat4{ 1.0f,1.0f,1.0f,1.0f,
		//						1.0f,1.0f,1.0f,1.0f,
		//						1.0f,1.0f,1.0f,1.0f,
		//						1.0f,1.0f,1.0f,1.0f };
	}

	void Camera2D::Init(int screenW, int screenH) {
		position = glm::vec2(0.0f, 0.0f);
		p_scale = 1.0f;
		p_needsMatrixUpdate = true;
		p_screenWidth = screenW;
		p_screenHeight = screenH;
		p_orthoMatrix = glm::ortho(0.0f, (float)p_screenWidth, 0.0f, (float)p_screenHeight);
	}

	void Camera2D::Update() {

		if (p_needsMatrixUpdate) {

			glm::vec3 translate(-position.x + p_screenWidth / 2, -position.y + p_screenHeight / 2, 0.0f);
			p_cameraMatrix = glm::translate(p_orthoMatrix, translate);

			glm::vec3 scale(p_scale, p_scale, 0.0f);
			p_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * p_cameraMatrix;

			p_needsMatrixUpdate = false;
		}
	}

	glm::vec2 &Camera2D::ConvertScreenToWorld(glm::vec2 &screenCoords) {
		//invert y direction
		screenCoords.y = p_screenHeight - screenCoords.y;
		// make it so that 0 is the center
		screenCoords = screenCoords - glm::vec2(p_screenWidth / 2, p_screenHeight / 2);
		//scale the coords
		screenCoords = screenCoords / p_scale;
		//translate the coords
		screenCoords = screenCoords + position;

		return screenCoords;
	}

	bool Camera2D::IsBoxInView(const glm::vec2& position, const glm::vec2 &dimensions) {

		glm::vec2 scaledScreenDimensions = glm::vec2(p_screenWidth , p_screenHeight) / p_scale ;

		const float MIN_DIST_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
		const float MIN_DIST_Y = (dimensions.y / 2.0f) + 200.0f + scaledScreenDimensions.y / 2.0f;

		glm::vec2 centerPos = position + dimensions/2.0f;
		glm::vec2 centerCameraPos = position;
		glm::vec2 distVec = centerPos - centerCameraPos;

		float xDepth = MIN_DIST_X - abs(distVec.x);
		float yDepth = MIN_DIST_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0) {

			//there was a collision
			return true;
		}
		return false;
	}
}