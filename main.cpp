//
//  main.cpp
//  OpenGL
//
//  Created by Joseph Despain on 11/28/13.
//  Copyright (c) 2013 Joseph Despain. All rights reserved.
//

#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>

#include "Program.h"



Program MyProgram;

void display();

int main(int argc, char** argv)
{
    MyProgram.Init(argc, argv);
    
    /*
     * Work around for call back
     * Will probably change to GLFW instead of GLUT
     */
    
    glutDisplayFunc(display);
    
    glutMainLoop();
   
    return 0;
}


void display()
{
    MyProgram.render();
}

