#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <functional>

#include "Shader.h"
#include "Camera.h"
#include "Color.h"
#include "Renderer.h"

struct WindowOptions {
	bool fullscreen = false;
	bool resizable = false;
	bool msaa = false; // Multisample anti-aliasing
	bool vsync = false; 
#ifdef _DEBUG
	bool debug = true;
#else
	bool debug = false;
#endif
};

struct Input {
	static bool isScrolling;
	static double scrollXOffset, scrollYOffset;
};

class Window {
public:
	Window(int width, int height, std::string title, WindowOptions options);
	~Window();

	inline bool Closed()					{ return glfwWindowShouldClose(mWin); }
	inline bool Error()						{ return mErrorStatus; }
	inline std::string GetError()			{ return mError; }
	inline std::string GetVersion()			{ return std::string((char*)glGetString(GL_VERSION)); }
	inline int GetFPS()						{ return mFPS; }
	inline Camera& GetCamera()				{ return mMainCamera; }

	void Update();
	void SetTitle(const std::string& title);
	void MakeCurrent();

	// Events
	bool KeyPressed(int key);
	bool KeyReleased(int key);

	static Input input;
private:
	static void ScrollCallbackFunc(GLFWwindow* window, double xOffset, double yOffset);
	static void ErrorCallbackFunc(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	bool mErrorStatus;
	Camera mMainCamera;
	std::string mError;
	GLFWwindow* mWin;
	float mFrames, mLastTime, mCurrentTime;
	int mFPS;
};