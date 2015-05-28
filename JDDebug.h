//
//  JDDebug.h
//  OpenGL
//
//  Created by Joseph Despain on 5/27/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__JDDebug__
#define __OpenGL__JDDebug__

#include <string>
#include <iostream>
#include <fstream>

class JDDebug
{
public:
    static JDDebug* Instance();
    bool OpenLog();
    bool CloseLog();
    void Log(std::string LogString);
    
private:
    JDDebug();
    ~JDDebug();
    static JDDebug* pInstance;
    std::ofstream* LogStream;
};
#endif /* defined(__OpenGL__JDDebug__) */
