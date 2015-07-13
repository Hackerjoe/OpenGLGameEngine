//
//  Program.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//



#include "Program.h"
#include <iostream>
#include "HIDManager.h"




Program::Program()
{
    ScreenWidth = 1280;
    ScreenHeight = 720;
    
}

float Program::testx = 0;
float Program::testy = 0;
float Program::testz = 5;



bool Program::Init(int argc, char** argv)
{
    
   
    //Init Glut and a window
   
    
    if (!glfwInit())
        return false;
    
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Hello World", NULL, NULL);
    
    
    glfwMakeContextCurrent(window);

    

    glEnable(GL_TEXTURE_2D);
    glClearColor (0.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    
    //Init devil for image reading
    ImageLibManager::Instance()->Init();
    
    //Setup glut callbacks
    //glutReshapeFunc(reshape);
    
    
    
    printf("\n%s\n", glGetString(GL_VERSION));
    
    //Check for support
    glewInit();
    
    if (glewIsSupported("GL_VERSION_2_1"))
        printf("Ready for OpenGL 2.1\n");
    else {
        printf("OpenGL 2.1 not supported\n");
        return false;
    }
    
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
        printf("Ready for GLSL - vertex, fragment\n");
    else {
        printf("Not Supported :(");
        return false;
    }
    
    PhysxManager::Instance()->Init();
    
    TheLevel = new Level();

    
    mainLoop();
    
        
    return true;
}

void Program::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        render();
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
}

void Program::render()
{
    

    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    glLoadIdentity();
    gluPerspective(45.,(GLfloat) width/(GLfloat) height,0.1, 800.0);
    
    gluLookAt(0,0,-1,
              0.0, 0.0, 0.0,
              0,1,0);
    
    glTranslatef(testx, testy, testz);
    glMatrixMode(GL_MODELVIEW);
    TheLevel->Update();
  
    
    
    
}

void Program::reshape(GLFWwindow* window, int width, int height)
{
    /*
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.,(GLfloat) width/(GLfloat) height,0.1, 800.0);

    gluLookAt(0.0,0.0,5.0, 0.0, 0.0, 0.0, 0,1,0);
    glMatrixMode(GL_MODELVIEW)*/

}

void Program::SetLevel(Level* level)
{
    TheLevel = level;
}

Program::~Program()
{
    
}
