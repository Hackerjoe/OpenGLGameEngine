//
//  JDFileManager.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/27/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "JDFileManager.h"

JDFileManager* JDFileManager::pInstance = NULL;

JDFileManager* JDFileManager::Instance()
{
    if (!pInstance)
    {
        pInstance = new JDFileManager();
    }
    
    return pInstance;
}

JDFileManager::JDFileManager()
{
    
}

JDFileManager::~JDFileManager()
{
    
}

bool JDFileManager::WriteToFile(const char *FileName, std::string Content)
{
    std::ofstream myfile;
    myfile.open(FileName);
    if (myfile.is_open())
    {
        myfile << Content;
        myfile.close();
        return true;
    }
    else
    {
        return false;
    }
    
}

std::string* JDFileManager::ReadFile(const char *FileName)
{
    std::string content;
    std::string line;
    std::ifstream myfile (FileName);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            content = content + line;
            std::cout << line << '\n';
        }
        return new std::string(content);
        myfile.close();
    }
    
    else
    {
        std::cout << "Unable to open file";
        return NULL;
    }
}