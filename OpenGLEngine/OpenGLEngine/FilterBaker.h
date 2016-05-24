#pragma once
#include "Shader.h"
#include "ImageLibManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <GLFW\glfw3.h>

using namespace std;

class FilterBaker
{
public:
	FilterBaker(std::vector<const GLchar*> faceImages,string outputName,GLint size);
	~FilterBaker(void);

	GLuint frameBuffer;
	GLuint colorAttachment;
	GLuint depthAttachment;


};

