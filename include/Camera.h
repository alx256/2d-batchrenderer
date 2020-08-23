#pragma once

#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"

class Camera {
public:
	Camera();
	~Camera();

	void UpdatePosition();
	void SetPosition(float xPosition, float yPosition);
	void SetPosition(const glm::vec2& position);
	void SetX(float position);
	void SetY(float position);
	void Translate(float xOffset, float yOffset);
	void Translate(const glm::vec2& offset);
	void Zoom(float xOffset, float yOffset);

	inline float GetX() const { return mX; }
	inline float GetY() const { return mY; }
	inline float GetScaleX() const { return mScaleX; }
	inline float GetScaleY() const { return mScaleY; }
	inline const glm::mat4& GetProjectionMatrix() const	{ return mProjection; }
	inline const glm::mat4& GetViewMatrix() const { return mView; }
	inline float GetProjectionWidth() const	{ return mProjectionWidth; }
	inline float GetProjectionHeight() const { return mProjectionHeight; }
	inline void SetProjectionWidth(float val) { mProjectionWidth = val; }
	inline void SetProjectionHeight(float val) { mProjectionHeight = val; }
private:
	glm::mat4 mProjection, mView;
	float mX, mY, mScaleX, mScaleY, mProjectionWidth, mProjectionHeight;
	bool mZoomUpdate, mPositionUpdate;
};