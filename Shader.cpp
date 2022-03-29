#include "Shader.h"
#include <SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader() :
	sShaderProgram(0),
	sVertexShader(0),
	sFragShader(0)
{}

Shader::~Shader() {}

bool Shader::Load(const std::string& vertName, const std::string& fragName) {
	if (!CompileShader(vertName, GL_VERTEX_SHADER, sVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, sFragShader)) {
		return false;
	}

	sShaderProgram = glCreateProgram();
	glAttachShader(sShaderProgram, sVertexShader);
	glAttachShader(sShaderProgram, sFragShader);
	glLinkProgram(sShaderProgram);

	if (!IsValidProgram()) {
		return false;
	}

	return true;
}

void Shader::Unload() {
	glDeleteProgram(sShaderProgram);
	glDeleteShader(sVertexShader);
	glDeleteShader(sFragShader);
}

void Shader::SetActive() {
	glUseProgram(sShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix) {
	GLuint loc = glGetUniformLocation(sShaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count) {
	GLuint loc = glGetUniformLocation(sShaderProgram, name);
	glUniformMatrix4fv(loc, count, GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector) {
	GLuint loc = glGetUniformLocation(sShaderProgram, name);
	glUniform3fv(loc, 1, reinterpret_cast<const float*>(&vector.x));
}

void Shader::SetFloatUniform(const char* name, float value) {
	GLuint loc = glGetUniformLocation(sShaderProgram, name);
	glUniform1f(loc, value);
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) {
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open()) {
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		outShader = glCreateShader(shaderType);
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader)) {
			SDL_Log("Shader Compile Error %s", fileName.c_str());
			return false;
		}
	}
	else {
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram() {

	GLint status;
	glGetProgramiv(sShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(sShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}
