#include "SubTexture.h"

#include <iostream>

SubTexture::SubTexture() {}

SubTexture::SubTexture(std::pair<float, float> dimensions, std::pair<float, float> coords) :
	mWidth(dimensions.first), mHeight(dimensions.second), mScaleToFit(true),
	mX(coords.first), mY(coords.second), mInitialized(false), mParentTexture(nullptr) {}

SubTexture::~SubTexture() {}

void SubTexture::SetParentTexture(const Texture& texture) {
	mTextureWidth = texture.GetWidth();
	mTextureHeight = texture.GetHeight();
	mParentTexture = &texture;
	mInitialized = true;

	if (mWidth > mTextureWidth || mHeight > mTextureHeight) {
		// The size is invalid
		std::cerr << "[ERROR] Invalid subtexture! Dimensions out of bounds with texture: " <<
			texture.GetPath() << '\n';
		mWidth = mTextureWidth;
		mHeight = mTextureHeight;
	}

	mNormalizedCoords = {
		{ (mX * mWidth) / mTextureWidth, (mY * mHeight) / mTextureHeight },
		{ ((mX + 1) * mWidth) / mTextureWidth, (mY * mHeight) / mTextureHeight },
		{ ((mX + 1) * mWidth) / mTextureWidth, ((mY + 1) * mHeight) / mTextureHeight },
		{ (mX * mWidth) / mTextureWidth, ((mY + 1) * mHeight) / mTextureHeight }
	};
}