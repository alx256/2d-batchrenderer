#include "Renderer.h"

RendererAttributes::RendererAttributes() {}

RendererAttributes::~RendererAttributes() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);
}

void RendererAttributes::Init() {
	maxQuadCount = 4000,
		maxVertexCount = maxQuadCount * 4,
		maxIndexCount = maxQuadCount * 6;

	// Generate Vertex Array
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Vertex Buffer
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, maxVertexCount * 36, nullptr, GL_DYNAMIC_DRAW);
	// Vertex Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (const GLvoid*)0);
	// Vertex Colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (const GLvoid*)(sizeof(float) * 2));
	// Texture Co-ordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (const GLvoid*)(sizeof(float) * 6));
	// Texture ID
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (const GLvoid*)(sizeof(float) * 8));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Index Buffer
	/* Repeating Pattern
		0 1 2 2 3 0
	*/
	int offset = 0;
	for (int i = 0; i < maxIndexCount; i += 6) {
		indices.push_back(0 + offset);
		indices.push_back(1 + offset);
		indices.push_back(2 + offset);
		indices.push_back(2 + offset);
		indices.push_back(3 + offset);
		indices.push_back(0 + offset);

		offset += 4;
	}

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	points[0] = 0.0f;
	points[1] = 1.0f;
	points[2] = 1.0f;
	points[3] = 0.0f;
	points[4] = 0.0f;
	points[5] = 0.0f;
	points[6] = 1.0f;
	points[7] = 1.0f;

	transformPointsX[0] = 0;
	transformPointsX[1] = 9;
	transformPointsX[2] = 18;
	transformPointsX[3] = 27;

	transformPointsY[0] = 1;
	transformPointsY[1] = 10;
	transformPointsY[2] = 19;
	transformPointsY[3] = 28;
}

void RendererAttributes::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

void RendererAttributes::UnBind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::Init() {
	mOffset = 0;
	mSize = 1;
	mAttributes.Init();
	mAttributes.textures = std::vector<GLint>(32);
	for (int i = 0; i < 32; i++)
		mAttributes.textures[i] = i;

	// Use the default shader
	mDefaultShader.Assign("../Engine/defaults/default.glsl");
	SetShader(mDefaultShader);
}

void Renderer::SetClearColor(const Color& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::StartNew(const Camera& camera) {
	SetShader(mDefaultShader);
	mDefaultShader.Uniform1iv("utextures", mAttributes.textures.size(), mAttributes.textures.data());
	mDefaultShader.UniformMat4f("uvp", camera.GetViewMatrix() * camera.GetProjectionMatrix());
	mDrawCalls = 0;
}

void Renderer::SetShader(const Shader& shader) {
	glUseProgram(shader.GetProgram());
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size) {
	DrawQuadReal(position, size, nullptr, nullptr, nullptr);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture) {
	DrawQuadReal(position, size, &texture, nullptr, nullptr);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture& subTexture) {
	DrawQuadReal(position, size, nullptr, &subTexture, nullptr);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color) {
	DrawQuadReal(position, size, nullptr, nullptr, &color);
}

void Renderer::DrawQuadReal(const glm::vec2& position,
	const glm::vec2& size,
	const Texture* texture,
	const SubTexture* subTexture,
	const Color* color) {
	if (mSize > mAttributes.maxQuadCount)
		Finish();

	mAttributes.vertices.insert(mAttributes.vertices.end(),
		{ 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f,	// Bottom Left
		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f,	// Bottom Right
		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f,	// Top Right
		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f }	// Top Left
	);

	PositionUpdate(position, size);

	if (subTexture != nullptr)
		SubTextureUpdate(*subTexture);
	else if (texture != nullptr)
		TextureUpdate(*texture);

	if (color != nullptr)
		ColorUpdate(*color);
	else
		ColorUpdate(Colors::White);

	mOffset = mAttributes.vertices.size();
	mSize++;
}

void Renderer::PositionUpdate(const glm::vec2& position, const glm::vec2& size) {
	// Position
	for (int i = 0; i <= 3; i++) {
		float transform = position.x + (mAttributes.points[i] * size.x);
		mAttributes.vertices[mAttributes.transformPointsX[i] + mOffset] = transform;
	}

	for (int i = 4; i <= 7; i++) {
		float transform = position.y + (mAttributes.points[i] * size.y);
		mAttributes.vertices[mAttributes.transformPointsY[i - 4] + mOffset] = transform;
	}
}

void Renderer::TextureUpdate(const Texture& texture) {
	texture.Bind();
	WriteTextureInfo(texture.GetID(), texture.GetCoords());
}

void Renderer::SubTextureUpdate(const SubTexture& subTexture) {
	if (!subTexture.IsInitialized()) {
		std::cerr << "[ERROR] Subtexture has not been initalized yet!\n";
		return;
	}

	subTexture.GetParentTexture()->Bind();
	WriteTextureInfo(subTexture.GetParentTexture()->GetID(), subTexture.GetNormalizedCoords());
}

void Renderer::ColorUpdate(const Color& color) {
	int add = 0;
	for (int i = 0; i < 4; i++) {
		mAttributes.vertices[mOffset + i + add + 2] = color.r;
		mAttributes.vertices[mOffset + i + add + 3] = color.g;
		mAttributes.vertices[mOffset + i + add + 4] = color.b;
		mAttributes.vertices[mOffset + i + add + 5] = color.a;

		add += 8;
	}
}

void Renderer::Finish() {
	mAttributes.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, mAttributes.vertices.size() * sizeof(GLfloat), mAttributes.vertices.data());
	glDrawElements(GL_TRIANGLES, mSize * 6, GL_UNSIGNED_INT, nullptr);
	mDrawCalls++;

	mSize = 1;
	mOffset = 0;
	mAttributes.vertices.clear();
}

void Renderer::WriteTextureInfo(int textureID, const floatvec& coords) {
	int add = 5;
	for (int i = 0; i < 4; i++) {
		// Texture coordinates
		mAttributes.vertices[mOffset + add + 1] = coords[i][0];
		mAttributes.vertices[mOffset + add + 2] = coords[i][1];
							  
		// Texture id		  
		mAttributes.vertices[mOffset + add + 3] = textureID;

		add += 9;
	}
}

Shader Renderer::mDefaultShader;
int Renderer::mSize;
int Renderer::mOffset;
int Renderer::mDrawCalls;
RendererAttributes Renderer::mAttributes;