//
//  HIDManager.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/27/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "HIDManager.h"

HIDManager* HIDManager::pInstance = NULL;
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

void HIDManager::KeyBoardFunc(unsigned char key, int x, int y)
{
    KeyStates[key] = true;
}

void HIDManager::KeyBoardUpFunc(unsigned char key, int x, int y)
{
    KeyStates[key] = false;
}

bool HIDManager::GetKey(char key)
{
    if (KeyStates[key] == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}