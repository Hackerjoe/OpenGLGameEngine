//
//  RenderComponent.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "RenderComponent.h"

RenderComponent::RenderComponent():Component()
{
    
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
    
    t+=1;
    glPushMatrix();
    glTranslatef(Parent->Postion->x, Parent->Postion->y, Parent->Postion->z);
    glRotatef(t, 1, 0, 0);
    glutWireTeapot(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
