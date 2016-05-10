//
//  Shader.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/21/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Shader.h"


Shader::Shader(std::string vert, std::string frag)
{
	if (!vert.empty() && !frag.empty())
	{
		VertFileName = new std::string(vert);
		FragFileName = new std::string(frag);
		CreateShader(vert, frag);
	}
	else
		CreateShader("Shaders/NullShader.vert","Shaders/NullShader.frag");

}

Shader::Shader()
{
	CreateShader("Shaders/NullShader.vert","Shaders/NullShader.frag");
}




void Shader::CreateShader(std::string vert, std::string frag)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	// OpenGL is C api so turn stuff into C style strings.
	char* vertShader = _strdup(vert.c_str());

	char* fragShader = _strdup(frag.c_str());

	if (vertShader != NULL && fragShader != NULL)
	{
		char *vs = NULL, *fs = NULL;

		// Create Shaders
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Read Shader files.
		vs = JDFileManager::Instance()->ReadFile(vertShader);
		fs = JDFileManager::Instance()->ReadFile(fragShader);

		const char * ff = fs;
		const char * vv = vs;

		if (ff == NULL || vs == NULL)
		{
			printf("Shader Null");
		}

		// Put in the shader sources.
		glShaderSource(vertexShader, 1, &vv, NULL);
		glShaderSource(fragmentShader, 1, &ff, NULL);

		// Free our allocated char pointers.
		free(vs);
		free(fs);

		// Compile glsl shaders.
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		
		// Print Errors if there are some.
		printShaderInfoLog(vertexShader);
		printShaderInfoLog(fragmentShader);

		// Create program.
		ShaderProgram = glCreateProgram();

		// Attach shaders to program.
		glAttachShader(this->ShaderProgram, fragmentShader);
		glAttachShader(this->ShaderProgram, vertexShader);

		//Link
		glLinkProgram(this->ShaderProgram);

		// Check for errors in program.
		printProgramInfoLog(this->ShaderProgram);

		// Delete our shaders they are in the program.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	else{
		printf("Shaders are NULL");
	}

}

void Shader::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("printShaderInfoLog: %s\n", infoLog);
		free(infoLog);
	}
}

void Shader::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("printProgramInfoLog: %s\n", infoLog);
		free(infoLog);
	}
}

void Shader::UseShader()
{
	glUseProgram(ShaderProgram);
}

GLuint Shader::GetShader()
{
	return ShaderProgram;
}

Shader::~Shader()
{

}