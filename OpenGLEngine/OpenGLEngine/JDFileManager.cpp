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

char* JDFileManager::ReadFile(const char *FileName)
{
	FILE *fp;
	char *content = NULL;
	int count = 0;

	if (FileName != NULL)
	{
		fp = fopen(FileName, "rt");


		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}

	}
	return content;
}