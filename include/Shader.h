#pragma once

#include <GL/glew.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

class Shader {
public:
	Shader();
	Shader(const Shader&);
	Shader(const std::string& path);
	~Shader();
	
	inline GLuint GetProgram() const { return mProgram; }
	void Assign(const std::string& _path);

	void Uniform4f(const std::string& uniform,
		float val1, float val2, float val3, float val4);
	void UniformMat4f(const std::string& uniform,
		const glm::mat4& matrix);
	void Uniform1iv(const std::string& uniform,
		GLsizei count, const GLint* value);
private:
	GLint UniformLocation(const std::string& uniform);
	void InitShader();
	bool ErrorOccured(GLuint shader, const std::string& type);

	std::ifstream mStream;
	std::string mVertexShaderSrc, mFragmentShaderSrc;
	std::string mPath;
	GLuint mProgram;
	std::unordered_map<std::string, GLint> mUniformCache;
};