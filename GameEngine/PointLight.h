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
#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

class PointLight
{
public:
    PointLight();
    glm::vec3 Position;
    glm::vec3 Color;
    float Linear;
    float Quadratic;
    float Radius;
    bool set;
    GLint PositionUni;
    GLint ColorUni;
    GLint LinearUni;
    GLint QuadraticUni;
    GLint RadiusUni;
    
    
    
    
    
private:
    
};

#endif /* defined(__OpenGL__PointLight__) */
