//
//  RenderComponent.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "RenderComponent.h"
#include "glm/ext.hpp"



RenderComponent::RenderComponent(Camera* camera, Shader NewShader, Mesh NewMesh ) :Component()
{

	CurrentCamera = camera;
	shader = new Shader(NewShader);
	
}

RenderComponent::~RenderComponent()
{

}

void RenderComponent::Update()
{
	Draw();
}

void RenderComponent::Start()
{

}

void RenderComponent::Draw()
{





	shader->UseShader();
	GLint modelLoc = glGetUniformLocation(shader->GetShader(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, Parent->Matrix);

	GLint viewLoc = glGetUniformLocation(shader->GetShader(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(CurrentCamera->View));

	GLint projLoc = glGetUniformLocation(shader->GetShader(), "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(CurrentCamera->Projection));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	glBindVertexArray(0);
}

