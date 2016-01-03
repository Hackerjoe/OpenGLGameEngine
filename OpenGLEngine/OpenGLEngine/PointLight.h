//
//  PointLight.h
//  OpenGL
//
//  Created by Joseph Despain on 8/14/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__PointLight__
#define __OpenGL__PointLight__
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "Shader.h"

class PointLight
{
public:
	PointLight();
	PointLight(glm::vec3 Position,float Radius,glm::vec3 Color);

	glm::mat4 ModelMatrix;
	glm::vec3 Position;
	glm::vec3 Color;
	float Radius;

private:

};

#endif /* defined(__OpenGL__PointLight__) */