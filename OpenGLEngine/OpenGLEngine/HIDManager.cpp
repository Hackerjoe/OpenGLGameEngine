//
//  HIDManager.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/27/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "HIDManager.h"
#include <iostream>

HIDManager* HIDManager::pInstance = nullptr;
bool* HIDManager::KeyStates = new bool[256];

HIDManager::HIDManager()
{
    
}

HIDManager::~HIDManager()
{
    
}

HIDManager* HIDManager::Instance()
{
    if(!pInstance)
    {
        pInstance = new HIDManager();
        
    }
    
    return pInstance;
}

void HIDManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   
    if (action == GLFW_PRESS)
        KeyStates[key] = true;
    
    if(action == GLFW_RELEASE)
        KeyStates[key] = false;
    

}

void HIDManager::character_callback(GLFWwindow* window, unsigned int codepoint)
{
    KeyStates[codepoint] = true;
}

bool HIDManager::GetKey(int key)
{
    return KeyStates[key];
}