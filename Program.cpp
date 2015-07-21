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
    ScreenWidth = 800;
    ScreenHeight = 600;
    
}

float Program::testx = 0;
float Program::testy = 0;
float Program::testz = 5;



bool Program::Init(int argc, char** argv)
{
    
   
   
    
    if (!glfwInit())
        return false;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Hello World", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    

    //glEnable(GL_TEXTURE_2D);
    glewExperimental = GL_TRUE;
    glewInit();
    
    
    //Init devil for image reading
    ImageLibManager::Instance()->Init();
    PhysxManager::Instance()->Init();
    
    

    
    
    //int width, height;
    //glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, ScreenWidth, ScreenHeight);
    
  
    
    TheLevel = new Level();
    
    
    mainLoop();
    
        
    return true;
}

void Program::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        render();
        
        
        
        
    }
}

void Program::render()
{
    

    testz+=2;
    
    
    glClearColor (0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
  

    TheLevel->Update();
    
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    
    
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
