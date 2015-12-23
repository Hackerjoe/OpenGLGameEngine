//
//  HIDManager.h
//  OpenGL
//
//  Created by Joseph Despain on 5/27/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__HIDManager__
#define __OpenGL__HIDManager__

#include <GLFW/glfw3.h>
#include <stdio.h>

class HIDManager
{
public:
    static HIDManager* Instance();
    static void KeyBoardFunc(unsigned char key, int x, int y);
    static void KeyBoardUpFunc(unsigned char key, int x, int y);
    static bool GetKey(int key);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void character_callback(GLFWwindow* window, unsigned int codepoint);
    
private:
     HIDManager();
    ~HIDManager();
    
    static HIDManager* pInstance;
    static bool* KeyStates;
    
};

#endif /* defined(__OpenGL__HIDManager__) */