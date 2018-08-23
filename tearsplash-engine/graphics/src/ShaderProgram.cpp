/**********************************************************************/
// File:	ShaderProgram.cpp
// Summary:	This class constructs a program carrying a set of shaders. The
//			caller can compile the shaders using file paths, and use them.
//
// Version: Version 1.0 - 2018-08-23
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-23 File created
/**********************************************************************/

// Includes -------------------------
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "ShaderProgram.h"
#include "Errors.h"

// ----------------------------------
// Default constructor
ShaderProgram::ShaderProgram() : mNumAttributes(0), mProgramID(0), mVertexShaderID(0), mFragmentShaderID(0)
{
	// Initialize member variables through MIL
}

// ----------------------------------
// Default destructor
ShaderProgram::~ShaderProgram()
{
	// Do nothing
}

// ----------------------------------
// Compiles a vertex shader and a fragment shader given
// file path for each file. This
void ShaderProgram::compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath)
{
	// Create program
	mProgramID = glCreateProgram();

	// Define local variables
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	// Create vertex and fragment shaders
	mVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	mFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (mVertexShaderID == (GLuint)0)
	{
		fatalError("Could not create vertex shader");
	}
	if (mFragmentShaderID == (GLuint)0)
	{
		fatalError("Could not create fragment shader");
	}

	// Compile shaders
	compileShader(vertexShaderFilePath, mVertexShaderID);
	compileShader(fragmentShaderFilePath, mFragmentShaderID);

	return;
}

// ----------------------------------
// Links a vertex shader and a fragment shader to the mProgramID
void ShaderProgram::linkShaders()
{
	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.

	// Attach our shaders to our program
	glAttachShader(mProgramID, mVertexShaderID);
	glAttachShader(mProgramID, mFragmentShaderID);

	// Link our program
	glLinkProgram(mProgramID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint success = 0;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mProgramID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(mProgramID);
		// Don't leak shaders either.
		glDeleteShader(mVertexShaderID);
		glDeleteShader(mFragmentShaderID);

		// Use the infoLog as you see fit.
		std::string errorMsg = "Failed to link program. Log info: ";//+ infoLog[0];
		fatalError(errorMsg);
	}

	// Always detach shaders after a successful link. Delete as well.
	glDetachShader(mProgramID, mVertexShaderID);
	glDetachShader(mProgramID, mFragmentShaderID);
	glDeleteShader(mVertexShaderID);
	glDeleteShader(mFragmentShaderID);

	return;
}

// ----------------------------------
// Compiles a shader given a file path and an id. Also prints errors
// if there are any.
void ShaderProgram::compileShader(const std::string& shaderFilePath, GLuint id)
{
	// Declare local variables
	std::string		shaderCode;
	std::ifstream	shaderFile;

	// Try opening files and reading into buffer:
	try
	{
		// Open files
		shaderFile.open(shaderFilePath);
		std::stringstream shaderStream;
		// Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// Convert stream into GLchar array
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		// There was an error open files and reading into stream
		std::string errorMsg = "Could not open vertex shader at path: " + shaderFilePath;
		fatalError(errorMsg);
	}
	const GLchar* shaderCodeCStr = shaderCode.c_str();;

	glShaderSource(id, 1, &shaderCodeCStr, nullptr);

	// Compile shader
	glCompileShader(id);

	// Print compile errors, if any, and exit.
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint maxLength = 0;

		// Get length of log message
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// Collect information data from compilation
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);
		std::string errorMsg = "Failed to compile shader at " + shaderFilePath + ". Log info: ";// +infoLog[0];
		glDeleteShader(id);
		fatalError(errorMsg);
	}

	return;
}

// ----------------------------------
// Compiles a shader given a file path and an id. Also prints errors
// if there are any.
void ShaderProgram::addAttribute(const std::string& attributeName)
{
	glBindAttribLocation(mProgramID, mNumAttributes++, attributeName.c_str());
}

// ----------------------------------
// Use program enable vertex attribute array
void ShaderProgram::use()
{
	glUseProgram(mProgramID);
	for (int i = 0; i < mNumAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

// ----------------------------------
// Don't use program anymore and disable vertex attribute array
void ShaderProgram::dontuse()
{
	glUseProgram(0);
	for (int i = 0; i < mNumAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}