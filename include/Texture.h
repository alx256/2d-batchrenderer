#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "stb_image.h"

using floatvec = std::vector<std::vector<float>>;

class Texture {
public:
	Texture();
	Texture(const std::string& _path);
	~Texture();

	void Assign(const std::string& _path);
	void Bind() const;
	void UnBind() const;

	inline GLuint GetID() const							{ return mID; }
	inline const std::string& GetPath() const			{ return mPath; }
	inline int GetWidth() const							{ return mWidth; }
	inline int GetHeight() const						{ return mHeight; }
	inline const floatvec& GetCoords() const			{ return mCoords; }

private:
	floatvec mCoords;
	int mWidth, mHeight, mBits;
	std::string mPath;
	GLuint mID;
};