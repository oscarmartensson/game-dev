// ShaderProgram.h

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <GL/glew.h>

namespace Tearsplash
{

	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		// Functions
		void compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath);
		void linkShaders();
		void addAttribute(const std::string& attributeName);
		void use();
		void dontuse();

		GLint getUniformLocation(const std::string& uniformName);

	private:
		int	   mNumAttributes;
		GLuint mProgramID;
		GLuint mVertexShaderID;
		GLuint mFragmentShaderID;

		void compileShader(const std::string& shaderFilePath, GLuint id);
	};

}

#endif // !SHADERPROGRAM_H