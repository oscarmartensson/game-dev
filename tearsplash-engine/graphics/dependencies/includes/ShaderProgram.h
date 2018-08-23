// ShaderProgram.h

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <GL/glew.h>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath);
	void linkShaders();
	void addAttribute(const std::string& attributeName);
	void use();
	void dontuse();

private:
	int	   mNumAttributes;
	GLuint mProgramID;
	GLuint mVertexShaderID;
	GLuint mFragmentShaderID;

	void compileShader(const std::string& shaderFilePath, GLuint id);
};

#endif // !SHADERPROGRAM_H