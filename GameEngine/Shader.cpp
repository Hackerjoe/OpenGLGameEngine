//
//  Shader.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/21/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "Shader.h"


Shader::Shader(std::string vert,std::string frag)
{
    if(!vert.empty() && !frag.empty())
    {
        CreateShader(vert, frag);
    }

}

void Shader::CreateShader(std::string vert,std::string frag)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    char* vertShader = strdup(vert.c_str());
    
    char* fragShader = strdup(frag.c_str());
    
    if(vertShader != NULL && fragShader != NULL)
    {
        char *vs = NULL, *fs = NULL;
        
        
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        
        
        vs = JDFileManager::Instance()->ReadFile(vertShader);
        fs = JDFileManager::Instance()->ReadFile(fragShader);
        
        const char * ff = fs;
        const char * vv = vs;
        
        
        glShaderSource(vertexShader, 1, &vv, NULL);
        glShaderSource(fragmentShader, 1, &ff, NULL);
        
        free(vs);
        free(fs);
        
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        //printf("Vertex\n");
        printShaderInfoLog(vertexShader);
        //printf("Fragment\n");
        printShaderInfoLog(fragmentShader);
        
        ShaderProgram = glCreateProgram();
        
        glAttachShader(this->ShaderProgram,fragmentShader);
        glAttachShader(this->ShaderProgram,vertexShader);
        
        //glProgramParameteriEXT(this->Program,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
        //glProgramParameteriEXT(this->Program,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
        
        //int temp;
        //glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
        ///glProgramParameteriEXT(this->Program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
        
        glLinkProgram(this->ShaderProgram);
        //glUseProgram(this->Program);
        printProgramInfoLog(this->ShaderProgram);
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }else{
        printf("Shaders are NULL");
    }
    
}

void Shader::printShaderInfoLog(GLuint obj)
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

void Shader::printProgramInfoLog(GLuint obj)
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

void Shader::UseShader()
{
    glUseProgram(ShaderProgram);
}

GLuint Shader::GetShader()
{
    return ShaderProgram;
}

Shader::~Shader()
{
    
}