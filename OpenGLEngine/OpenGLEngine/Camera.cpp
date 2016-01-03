//
//  Camera.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/21/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Camera.h"

Camera::Camera(const GLuint width, const GLuint height)
{
	Width = width;
	Height = height;
	CameraPosition.x = 0;
	CameraPosition.y = 0;
	CameraPosition.z = 0;
	View = glm::lookAt(glm::vec3(0, 0, -10), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);


}

void Camera::Update()
{
	View = glm::lookAt(glm::vec3(0, 0, -10), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
}