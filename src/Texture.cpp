#include "Texture.h"

#include <iostream>

Texture::Texture() {
	mID = -1;
}

Texture::Texture(const std::string& _path) {
	Assign(_path);
}

Texture::~Texture() {
	glDeleteTextures(1, &mID);
}

void Texture::Assign(const std::string& _path) {
	mPath = _path;
	mWidth = 0;
	mHeight = 0;
	mBits = 0;
	mCoords = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};

	stbi_set_flip_vertically_on_load(1);
	unsigned char* pixels = stbi_load(mPath.c_str(), &mWidth, &mHeight, &mBits, 4);

	if (pixels == nullptr)
		std::cout << "Error opening texture \"" << mPath << "\"\n";

	glCreateTextures(GL_TEXTURE_2D, 1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);
}

void Texture::Bind() const {
	glBindTextureUnit(mID, mID);
}

void Texture::UnBind() const {
	glBindTextureUnit(mID, 0);
}