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
    
    t+=1;
    glPushMatrix();
    float mat[16];
    
    /*
     * I am finding out how 4x4 model view matrices work
     */
    
    mat[0] = 1;
    mat[1] = 0;
    mat[2] = 0;
    mat[3] = 0;
    
    mat[4] = 0;
    mat[5] = 1;
    mat[6] = 0;
    mat[7] = 0;
    
    mat[8] = 0;
    mat[9] = 0;
    mat[10] = 1;
    mat[11] = 0;
    //Position
    mat[12] = 0;
    mat[13] = 0;
    mat[14] = 0;
    mat[15] = 1;
    
    //glTranslatef(Parent->Postion->x, Parent->Postion->y, Parent->Postion->z);
    //glRotatef(t, 1, 0, 0);
    glMultMatrixf(mat);
    glRotatef(20, 1, 0,0);
    float m[16];
    glGetFloatv (GL_MODELVIEW_MATRIX, m);
    
    glutWireTeapot(1);
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
