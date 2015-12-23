//
//  ImageLibManager.h
//  OpenGL
//
//  Created by Joseph Despain on 7/8/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef __OpenGL__ImageLibManager__
#define __OpenGL__ImageLibManager__
#include <GL/glew.h>
#include <OpenGL/glu.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>


class ImageLibManager
{

    
public:
    static ImageLibManager* Instance();
    void Init();
    GLuint loadImage(const char* theFileName);
    GLuint loadCubemap(std::vector<const GLchar*> faces);
private:
    
    ImageLibManager();
    ~ImageLibManager();
    
    static ImageLibManager* pInstance;
    
    
};

#endif /* defined(__OpenGL__ImageLibManager__) */
