
//
//  Shader.h
//  OpenGL
//
//  Created by Joseph Despain on 7/21/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H

#include <gl/glew.h>
#include <string>
#include "JDFileManager.h"

class Shader
{
public:
	Shader(std::string vert, std::string frag);
	Shader();
	~Shader();

	void CreateShader(std::string vert, std::string frag);
	void UseShader();
	GLuint GetShader();
private:
	GLuint ShaderProgram;
	std::string* VertFileName;
	std::string* FragFileName;
	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);

};
#endif /* defined(__OpenGL__Shader__) */