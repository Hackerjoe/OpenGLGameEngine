//
//  TestInstancingComp.h
//  OpenGL
//
//  Created by Joseph Despain on 7/20/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__TestInstancingComp__
#define __OpenGL__TestInstancingComp__

#include "Component.h"
#include <GL/glew.h>
#include "JDMath.h"
#include "JDFileManager.h"
#include <string>
#include <sstream>

class TestInstComp : public Component
{
public:
    TestInstComp();
    ~TestInstComp();
    void Update();
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    void setShaders(std::string vert,std::string frag);
    GLuint Vertex;
    GLuint Fragment;
    GLuint vboId;
    GLuint Program;
    
};

#endif /* defined(__OpenGL__TestInstancingComp__) */
