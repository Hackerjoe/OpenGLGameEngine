//
//  RenderComponent.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "RenderComponent.h"
#include <math.h>

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
    
    //t+=1;
    glPushMatrix();
    //glTranslatef(Parent->Postion->x, Parent->Postion->y, Parent->Postion->z);
    //glRotatef(t, 1, 0, 0);
    glMultMatrixf(Parent->Matrix);
   // glRotatef(20, 1, 0,0);
    //float m[16];
    //glGetFloatv (GL_MODELVIEW_MATRIX, m);
    
    glutWireTeapot(1);
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
