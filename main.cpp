//
//  main.cpp
//  OpenGL
//
//  Created by Joseph Despain on 11/28/13.
//  Copyright (c) 2013 Joseph Despain. All rights reserved.
//

#include <iostream>
#include "GL/glew.h"
#include <GL/freeglut.h>
//#include <stdio.h>
//#include <AL/alut.h>
//#include <stdlib.h>
//#include <IL/il.h>
//#include <IL/ilu.h>
//#include <IL/ilut.h>
//#include <fstream>
#include "Program.h"

struct color4
{
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
    float a = 0.0;
};

void display();
void reshape(int width, int height);
void idle();
void keyb(unsigned char key, int x, int y);
bool fullScreen = false;
void motionPassive(int x, int y);

GLuint loadImage(const char* theFileName);
void setDiffuseColor(GLuint programID, color4 color);
void setShaders(std::string vert,std::string frag);
void setDiffuseTexture(GLuint programID, GLuint texture);
int textFileWrite(char *fn, char *s);


GLuint v,f,p;
GLenum texture;

float angle = 0;
float colorChange = 0;

Program MyProgram;


int main(int argc, char** argv)
{
    MyProgram.Init(argc, argv);
    
    //Work around to render.
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMainLoop();
   
    //glutReshapeFunc(reshape);
    
    //glutKeyboardFunc(keyb);
    
    //glutPassiveMotionFunc(motionPassive);
    
    
    
    //GLuint TextureHandle = loadImage("test.png");
    //setShaders("first.vert","first.frag");
    //setDiffuseTexture(p, TextureHandle);

    // setDiffuseColor(p, dC);
    
    return 0;
}


void display()
{
    MyProgram.render();
}

/*

char* textFileRead(char *fileName)
{
    FILE *fp;
    char *content = NULL;
    int count = 0;
    
    if (fileName != NULL)
    {
        fp = fopen(fileName, "rt");
        
        
        if (fp != NULL)
        {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
        
    }
    
    
    return content;
}



void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("printShaderInfoLog: %s\n",infoLog);
        free(infoLog);
    }else{
        printf("Shader Info Log: OK\n");
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("printProgramInfoLog: %s\n",infoLog);
        free(infoLog);
    }else{
        printf("Program Info Log: OK\n");
    }
}

void setShaders(std::string vert,std::string frag)
{
    char* vertShader = strdup(vert.c_str());
    
    char* fragShader = strdup(frag.c_str());
    
    if(vertShader != NULL && fragShader != NULL)
    {
        char *vs = NULL, *fs = NULL;
        
        
        v = glCreateShader(GL_VERTEX_SHADER);
        f = glCreateShader(GL_FRAGMENT_SHADER);
        
        
        
        vs = textFileRead(vertShader);
        fs = textFileRead(fragShader);
        
        const char * ff = fs;
        const char * vv = vs;
        
        
        glShaderSource(v, 1, &vv, NULL);
        glShaderSource(f, 1, &ff, NULL);
        
        free(vs);
        free(fs);
        
        glCompileShader(v);
        glCompileShader(f);
        
        p = glCreateProgram();
        
        glAttachShader(p,f);
        glAttachShader(p,v);
        
        glProgramParameteriEXT(p,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
        glProgramParameteriEXT(p,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
        
        int temp;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
        glProgramParameteriEXT(p,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
        
        glLinkProgram(p);
        glUseProgram(p);
        
        printShaderInfoLog(v);
        printShaderInfoLog(f);
        printProgramInfoLog(p);
    }else{
        printf("Shaders are NULL");
    }
    
}


GLuint loadImage(const char* theFileName)
{
    ILuint imageID;				// Create an image ID as a ULuint
    
    GLuint textureID;			// Create a texture ID as a GLuint
    
    ILboolean success;			// Create a flag to keep track of success/failure
    
    ILenum error;				// Create a flag to keep track of the IL error state
    
    ilGenImages(1, &imageID); 		// Generate the image ID
    
    ilBindImage(imageID); 			// Bind the image
    
    success = ilLoadImage(theFileName); 	// Load the image file
    
    // If we managed to load the image, then we can start to do things with it...
    if (success)
    {
        // If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
        ILinfo ImageInfo;
        iluGetImageInfo(&ImageInfo);
        if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
        {
            iluFlipImage();
        }
        
        // Convert the image into a suitable format to work with
        // NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
        success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
        
        // Quit out if we failed the conversion
        if (!success)
        {
            error = ilGetError();
            std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
            exit(-1);
        }
        
        // Generate a new texture
        glGenTextures(1, &textureID);
        
        // Bind the texture to a name
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        
        // Set texture interpolation method to use linear interpolation (no MIPMAPS)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        // Specify the texture specification
        glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
                     0,				// Pyramid level (for mip-mapping) - 0 is the top level
                     ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
                     ilGetInteger(IL_IMAGE_WIDTH),	// Image width
                     ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
                     1,				// Border width in pixels (can either be 1 or 0)
                     ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
                     GL_UNSIGNED_BYTE,		// Image data type
                     ilGetData());			// The actual image data itself
    }
    else // If we failed to open the image file in the first place...
    {
        error = ilGetError();
        std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
        exit(-1);
    }
    
    ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
    
    std::cout << "Texture creation successful." << std::endl;
    
    return textureID; // Return the GLuint to the texture so you can use it!
}

void setDiffuseTexture(GLuint programID, GLuint texture)
{
    GLuint loc = glGetUniformLocation(programID,"colorMap");
    
    glUniform1i(loc,0);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    
    glBindTexture(GL_TEXTURE_2D, texture);
}

void setNormalMap(GLuint programID, GLuint normalTexture)
{
    GLuint loc = glGetUniformLocation(programID,"normalMap");
    
    glUniform1i(loc,2);
    
    glActiveTexture(GL_TEXTURE0 + 2);
    
    glBindTexture(GL_TEXTURE_2D, texture);
}

void setDiffuseColor(GLuint programID, color4 color)
{
    GLuint diffuseLoc = glGetUniformLocation(programID, "diffuseColor");
    
    glUniform4f(diffuseLoc,color.r,color.g,color.b,color.a);
}
*/