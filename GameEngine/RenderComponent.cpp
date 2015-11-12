//
//  RenderComponent.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "RenderComponent.h"
#include "glm/ext.hpp"



RenderComponent::RenderComponent(Camera* cCamera,Shader* mShader, Model* mModel):Component()
{
 
    CurrentCamera = cCamera;
    shader = mShader;
    model = mModel;


    
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
    model->Draw(*shader);

}



