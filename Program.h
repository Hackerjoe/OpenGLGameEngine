//
//  Program.h
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <stdio.h>
//#include <AL/alut.h>
#include <stdlib.h>
#include "ImageLibManager.h"
#include <vector>
#include "Entity.h"
#include "Level.h"


#include "PhysxManager.h"

class Program
{
public:
    
    int ScreenWidth;
    int ScreenHeight;
    Level* TheLevel;
    
    
    Program();
    ~Program();
    
    bool Init(int argc, char** argv);
    void render();
    static void reshape(int width, int height);
    static void keyb(unsigned char key, int x, int y);
    void SetLevel(Level* level);
    static float testx;
    static float testy;
    static float testz;
    int Frame;
    float CurrentTime;
    float PrevTime;
    float fps;
    
    
private:

    
    
};
#endif