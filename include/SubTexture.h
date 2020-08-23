#pragma once

#include <utility>

#include "Texture.h"

class SubTexture {
public:
	SubTexture();
	SubTexture(std::pair<float, float> dimensions, std::pair<float, float> coords);
	~SubTexture();

	void SetParentTexture(const Texture& texture);

	inline bool ScaleToFit() const						{ return mScaleToFit; }
	inline float GetWidth() const						{ return mWidth; }
	inline float GetHeight() const						{ return mHeight; }
	inline float GetX() const							{ return mX; }
	inline float GetY()	const							{ return mY; }
	inline const Texture* GetParentTexture() const		{ return mParentTexture; }
	inline bool IsInitialized() const					{ return mInitialized; }
	inline const floatvec& GetNormalizedCoords() const	{ return mNormalizedCoords; }
	inline void SetScaleToFit(bool val)					{ mScaleToFit = val; }
	inline void SetTextureWidth(float val)				{ mTextureWidth = val; }
	inline void SetTextureHeight(float val)				{ mTextureHeight = val; }

private:
	float mTextureWidth, mTextureHeight, mX, mY, mWidth, mHeight;
	floatvec mNormalizedCoords;
	bool mScaleToFit;
	mutable bool mInitialized;
	const Texture* mParentTexture;
};