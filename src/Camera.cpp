#include "Camera.h"

Camera::Camera() {
	mView = glm::mat4(1.0f);
	mX = 0.0f;
	mY = 0.0f;
	mScaleX = 1.0f;
	mScaleY = 1.0f;
	mPositionUpdate = true;
	mZoomUpdate = true;
}

Camera::~Camera() {}

void Camera::SetPosition(float xPosition, float yPosition) {
	mX = xPosition;
	mY = yPosition;
	mPositionUpdate = true;
}

void Camera::SetPosition(const glm::vec2& position) {
	mX = position.x;
	mY = position.y;
	mPositionUpdate = true;
}

void Camera::Translate(float xOffset, float yOffset) {
	mX += xOffset;
	mY += yOffset;
	mPositionUpdate = true;
}

void Camera::Translate(const glm::vec2& offset) {
	mX += offset.x;
	mY += offset.y;
	mPositionUpdate = true;
}

void Camera::SetX(float position) {
	mX = position;
	mPositionUpdate = true;
}

void Camera::SetY(float position) {
	mY = position;
	mPositionUpdate = true;
}

void Camera::Zoom(float xOffset, float yOffset) {
	mScaleX += xOffset;
	mScaleY += yOffset;
	mZoomUpdate = true;
}

void Camera::UpdatePosition() {
	if (mPositionUpdate) {
		mView = glm::translate(glm::mat4(1.0f), glm::vec3(-mX, mY, 0.0f));
		mPositionUpdate = false;
	}

	if (mZoomUpdate) {
		mProjection = glm::ortho(
			0.0f,
			mProjectionWidth * mScaleX,
			0.0f,
			mProjectionHeight * mScaleY,
			-1.0f, 1.0f
		);
		mZoomUpdate = false;
	}
}