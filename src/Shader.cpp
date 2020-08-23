#include "Shader.h"

#include <iostream>

Shader::Shader() {}
Shader::Shader(const Shader& ) {}

Shader::Shader(const std::string& path) {
	Assign(path);
}

Shader::~Shader() {}

void Shader::InitShader() {
	mStream.open(mPath);
	if (!mStream) {
		std::cerr << "Couldn't open shader file " << mPath << '\n';
		return;
	}

	std::string line;
	bool fragment = false, vertex = false;
	GLuint vertexShader, fragmentShader;
	std::string vertexShaderSrc, fragmentShaderSrc;
	int linePlace = 0;

	mProgram = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read in the file
	while (std::getline(mStream, line)) {
		linePlace++;

		// Ignore empty lines
		if (line.empty()) continue;
		// Ignore comments
		if (line.substr(0, 2) == "//") continue;

		if (line == "VERTEX SHADER") {
			fragment = false;
			vertex = true;
			continue;
		}

		if (line == "FRAGMENT SHADER") {
			fragment = true;
			vertex = false;
			continue;
		}

		if (vertex) {
			vertexShaderSrc += line + '\n'; continue;
		}

		if (fragment) {
			fragmentShaderSrc += line + '\n'; continue;
		}

		// If the shader does not include VERTEX SHADER or FRAGMENT SHADER
		std::cout << "Error creating shader!\n" <<
			"Did not specify shader type on line " << linePlace << '\n';
		return;
	}

	const char* vss = vertexShaderSrc.c_str(),
		* fss = fragmentShaderSrc.c_str();

	glShaderSource(vertexShader, 1, &vss, nullptr);
	glShaderSource(fragmentShader, 1, &fss, nullptr);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	
	if (ErrorOccured(vertexShader, "Vertex Shader") || ErrorOccured(fragmentShader, "Fragment Shader")) {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	glAttachShader(mProgram, vertexShader);
	glAttachShader(mProgram, fragmentShader);
	glLinkProgram(mProgram);
	glValidateProgram(mProgram);

	glDetachShader(mProgram, vertexShader);
	glDetachShader(mProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Assign(const std::string& _path) {
	mPath = _path;
	InitShader();
}

GLint Shader::UniformLocation(const std::string& uniform) {
	if (mUniformCache.find(uniform) == mUniformCache.end()) {
		mUniformCache[uniform] = glGetUniformLocation(mProgram, uniform.c_str());
		if (mUniformCache[uniform] == -1)
			std::cerr << "Invalid uniform name " << uniform << "!\n";
	}
	return mUniformCache[uniform];
}

void Shader::Uniform4f(const std::string& uniform, float val1, float val2, float val3, float val4) {
	glUniform4f(UniformLocation(uniform), val1, val2, val3, val4);
}

void Shader::UniformMat4f(const std::string& uniform, const glm::mat4& matrix) {
	glUniformMatrix4fv(UniformLocation(uniform), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::Uniform1iv(const std::string& uniform, GLsizei count, const GLint* value) {
	glUniform1iv(UniformLocation(uniform), count, value);
}

bool Shader::ErrorOccured(GLuint shader, const std::string& shader_type) {
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		std::cerr << "Error while compiling shader " << shader_type << '\n';
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << message;
		delete[] message;
		return true;
	}

	return false;
}