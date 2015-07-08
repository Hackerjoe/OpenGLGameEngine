//
//  RenderComponent.h
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//
#ifndef RENDERNCOMP_H
#define RENDERNCOMP_H

#include "Component.h"
#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>
#include "HIDManager.h"
#include "JDFileManager.h"
#include "JDMath.h"

class RenderComponent : public Component
{
public:
    RenderComponent(std::string VetexShaderFileLoc,std::string FragmentShaderFileLoc);
    ~RenderComponent();
    
    virtual void Update();
    virtual void Start();
    
    void setShaders(std::string vert,std::string frag);
    void setDiffuseColor(GLuint programID, Color color);
    void setDiffuseTexture(GLuint programID, GLuint texture);
    GLuint Program;
    
private:
    void Draw();
    
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    GLuint Vertex;
    GLuint Fragment;

    
    
};


#endif