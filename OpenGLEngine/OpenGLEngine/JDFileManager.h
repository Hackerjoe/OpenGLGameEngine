//
//  JDFileManager.h
//  OpenGL
//
//  Created by Joseph Despain on 5/27/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//
// JDFileManager is a singleton class. One can only made.

#ifndef __OpenGL__JDFileManager__
#define __OpenGL__JDFileManager__
#define _CRT_SECURE_NO_DEPRECATE
#include <fstream>
#include <string>
#include <iostream>

class JDFileManager
{
public:
	static JDFileManager* Instance();
	bool WriteToFile(const char *FileName, std::string Content);
	char* ReadFile(const char *FileName);
private:
	JDFileManager();
	~JDFileManager();
	static JDFileManager* pInstance;

};

#endif /* defined(__OpenGL__JDFileManager__) */