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
    MainCamera = new Camera(ScreenWidth,ScreenHeight);
    
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
    
  
    
    TheLevel = new Level(ScreenWidth,ScreenHeight);
    
    
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
    
    glClearColor (0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    TheLevel->Update();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    
    
}

void Program::reshape(GLFWwindow* window, int width, int height)
{
    
}

void Program::SetLevel(Level* level)
{
    TheLevel = level;
}

Program::~Program()
{
    
}
