//
//  PointLight.cpp
//  OpenGL
//
//  Created by Joseph Despain on 8/14/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "PointLight.h"

PointLight::PointLight()
{
	this->Radius = 1.0f;
	this->Position = glm::vec3(0,0,0);
	this->Color = glm::vec3(1,1,1);
	
	this->ModelMatrix = glm::translate(ModelMatrix, this->Position);
	this->ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
}

