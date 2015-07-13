//
//  RenderComponent.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "RenderComponent.h"
#include <math.h>

RenderComponent::RenderComponent(std::string VetexShaderFileLoc,std::string FragmentShaderFileLoc):Component()
{
    if(!VetexShaderFileLoc.empty() && !FragmentShaderFileLoc.empty())
    {
        setShaders(VetexShaderFileLoc, FragmentShaderFileLoc);
        
    }
}
RenderComponent::~RenderComponent()
{
    
}
void RenderComponent::Update()
{
    Draw();
}
void RenderComponent::Start()
{
 
}

void RenderComponent::Draw()
{
    
    //t+=1;
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    //glLinkProgram(this->Program);
    glUseProgram(this->Program);
    //glTranslatef(Parent->Postion->x, Parent->Postion->y, Parent->Postion->z);
    //glRotatef(t, 1, 0, 0);
    glMultMatrixf(Parent->Matrix);
   // glRotatef(20, 1, 0,0);
    //float m[16];
    //glGetFloatv (GL_MODELVIEW_MATRIX, m);
    
    //glutSolidOctahedron();
    //glDisable(GL_TEXTURE_2D);
    
    
    
    GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    
    glScalef(.5, .5, .5);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, g_vertex_buffer_data);
    glDrawArrays(GL_TRIANGLES, 0, 3*12);
    glDisable(GL_VERTEX_ARRAY);
    //glutWireTeapot(1);
    

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
}


void RenderComponent::setShaders(std::string vert,std::string frag)
{
    char* vertShader = strdup(vert.c_str());
    
    char* fragShader = strdup(frag.c_str());
    
    if(vertShader != NULL && fragShader != NULL)
    {
        char *vs = NULL, *fs = NULL;
        
        
        this->Vertex = glCreateShader(GL_VERTEX_SHADER);
        this->Fragment = glCreateShader(GL_FRAGMENT_SHADER);
        
        
        
        vs = JDFileManager::Instance()->ReadFile(vertShader);
        fs = JDFileManager::Instance()->ReadFile(fragShader);
        
        const char * ff = fs;
        const char * vv = vs;
        
        
        glShaderSource(this->Vertex, 1, &vv, NULL);
        glShaderSource(this->Fragment, 1, &ff, NULL);
        
        free(vs);
        free(fs);
        
        glCompileShader(this->Vertex);
        glCompileShader(this->Fragment);
        
        this->Program = glCreateProgram();
        
        glAttachShader(this->Program,this->Fragment);
        glAttachShader(this->Program,this->Vertex);
        
        glProgramParameteriEXT(this->Program,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
        glProgramParameteriEXT(this->Program,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
        
        int temp;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
        glProgramParameteriEXT(this->Program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
        
        glLinkProgram(this->Program);
        glUseProgram(this->Program);
        
        printShaderInfoLog(this->Vertex);
        printShaderInfoLog(this->Fragment);
        printProgramInfoLog(this->Program);
    }else{
        printf("Shaders are NULL");
    }
    
}

void RenderComponent::printShaderInfoLog(GLuint obj)
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

void RenderComponent::printProgramInfoLog(GLuint obj)
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

void RenderComponent::setDiffuseColor(GLuint programID, Color color)
{
    GLuint diffuseLoc = glGetUniformLocation(programID, "Color");
    
    glUniform4f(diffuseLoc,color.r,color.g,color.b,color.a);
}

void RenderComponent::setDiffuseTexture(GLuint programID, GLuint texture)
{
    GLuint loc = glGetUniformLocation(programID,"texture1");
    std::cout << "LOC" << loc << std::endl;
    
    glUniform1i(loc,0);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    
    glBindTexture(GL_TEXTURE_2D, texture);
}

