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
#include <GL/glew.h>
#include <vector>
//#include <OpenGL/OpenGL.h>
//#include <GLFW/glfw3.h>

#include "HIDManager.h"
#include "JDFileManager.h"
#include "JDMath.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

class RenderComponent : public Component
{
public:
    
    RenderComponent(Camera* cCamera,Shader* mShader, Model* mModel);
    
    ~RenderComponent();
    
    virtual void Update();
    virtual void Start();
    
    Shader* shader;
    Model* model;

private:
    void Draw();

    Camera* CurrentCamera;
    
    
    
    //std::vector<unsigned int>* indices;

    
    
};


#endif