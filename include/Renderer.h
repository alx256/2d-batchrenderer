#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Color.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "SubTexture.h"

class RendererAttributes {
public:
	RendererAttributes();
	~RendererAttributes();

	void Init();

	void Bind();
	void UnBind();

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<GLint> textures;
	int texture_num, transformPointsX[4], transformPointsY[4], maxQuadCount,
		maxVertexCount, maxIndexCount;
	GLuint vb /* Vertex Buffer */, ib /* Index Buffer */, vao /* Vertex Array Object */;
	float projectionWidth, projectionHeight, points[8];
};

class Renderer {
public:
	Renderer();
	Renderer(const Renderer&) = delete;
	~Renderer();

	static void Init();
	static void StartNew(const Camera& camera);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& subTexture);
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color);
	static void SetClearColor(const Color& color);
	static void Finish();
	static void Clear();
	static void SetShader(const Shader& shader);

	static inline int GetDrawCallCount()					{ return mDrawCalls; }
	static inline RendererAttributes& GetAttributes()			{ return mAttributes; }
private:
	static void DrawQuadReal(const glm::vec2& position,
		const glm::vec2& size,
		const Texture* texture,
		const SubTexture* subTexture,
		const Color* color);
	static void PositionUpdate(const glm::vec2& position, const glm::vec2& size);
	static void TextureUpdate(const Texture& texture);
	static void SubTextureUpdate(const SubTexture& subTexture);
	static void WriteTextureInfo(int textureID, const floatvec& coords);
	static void ColorUpdate(const Color& color);
	
	static Shader mDefaultShader;
	static int mSize, mOffset, mDrawCalls;
	static RendererAttributes mAttributes;
};