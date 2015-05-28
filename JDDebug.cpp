//
//  JDDebug.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/27/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "JDDebug.h"

JDDebug* JDDebug::pInstance = NULL;

JDDebug* JDDebug::Instance()
{
    if(!pInstance)
    {
        pInstance = new JDDebug();
    }
    return pInstance;
}

JDDebug::JDDebug()
{
    LogStream = new std::ofstream();
    OpenLog();
}

JDDebug::~JDDebug()
{
    
}

bool JDDebug::OpenLog()
{
    
    LogStream->open("log.txt");
    if (LogStream->is_open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool JDDebug::CloseLog()
{
    LogStream->close();
    if(LogStream->is_open())
    {
        return false;
    }
    else{
        return true;
    }
}

void JDDebug::Log(std::string LogString)
{
    std::cout << LogString << "\n";
    *LogStream << LogString;
}
