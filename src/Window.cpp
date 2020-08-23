#include "Window.h"

#include <iostream>

bool Input::isScrolling = false;
double Input::scrollXOffset = 0.0;
double Input::scrollYOffset = 0.0;

Window::Window(int width, int height, std::string title, WindowOptions options) : mErrorStatus(false) {
	if (!glfwInit()) {
		mError = true;
		mErrorStatus = "Failed to initialise GLFW!";
		return;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, options.debug);
	
	if (options.msaa)
		glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, options.resizable);

	mWin = glfwCreateWindow(width, height, title.c_str(),
		(options.fullscreen) ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	
	if (!mWin) {
		mErrorStatus = true;
		mError = std::string("Failed to create window! ");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(mWin);

	// Initialise GLEW
	if (glewInit() != GLEW_OK) {
		mErrorStatus = true;
		mError = "Failed to initialise GLEW!";
		glfwTerminate();
		return;
	}

	if (options.msaa)
		glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Window::ErrorCallbackFunc, nullptr);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetScrollCallback(mWin, Window::ScrollCallbackFunc);

	if (options.vsync)
		glfwSwapInterval(1);

	Renderer::Init();
	mMainCamera = Camera();
	mMainCamera.SetProjectionWidth(width);
	mMainCamera.SetProjectionHeight(height);
}

Window::~Window() {
	glfwDestroyWindow(mWin);
	glfwTerminate();
}

// TODO: Decrease GPU usage
void Window::Update() {
	Window::input.isScrolling = false;
	Window::input.scrollXOffset = 0.0;
	Window::input.scrollYOffset = 0.0;

	// Set FPS
	mCurrentTime = glfwGetTime();
	if (mCurrentTime - mLastTime >= 1.0f) {
		mLastTime = mCurrentTime;
		mFPS = mFrames;
		mFrames = 0;
	}
	mFrames++;

	glfwSwapBuffers(mWin);
	glfwPollEvents();
}

void Window::SetTitle(const std::string& title) {
	glfwSetWindowTitle(mWin, title.c_str());
}

bool Window::KeyPressed(int key) {
	return glfwGetKey(mWin, key) == GLFW_PRESS;
}

bool Window::KeyReleased(int key) {
	return glfwGetKey(mWin, key) == GLFW_RELEASE;
}

void Window::ErrorCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cerr << "Internal OpenGL error: " << message << '\n';
}

void Window::ScrollCallbackFunc(GLFWwindow* window, double xOffset, double yOffset) {
	Window::input.isScrolling = true;
	Window::input.scrollXOffset = xOffset;
	Window::input.scrollYOffset = yOffset;
}

void Window::MakeCurrent() {
	glfwMakeContextCurrent(mWin);
}