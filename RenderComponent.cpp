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
                                // ID of VBO
    
    GLfloat vertices[] = {
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
    
 
    
    

    
   /* GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left
        0.5f, -0.5f, 0.0f, // Right
        0.0f,  0.5f, 0.0f  // Top
    };*/
    
    //GLuint VAO;
    //GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

    

    
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
    //glScalef(.5, .5, .5);
    
    glUseProgram(shaderProgram);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, Parent->Matrix);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3*12);
    glBindVertexArray(0);
    
    
}


void RenderComponent::setShaders(std::string vert,std::string frag)
{
    char* vertShader = strdup(vert.c_str());
    
    char* fragShader = strdup(frag.c_str());
    
    if(vertShader != NULL && fragShader != NULL)
    {
        char *vs = NULL, *fs = NULL;
        
        
        this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
        this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        
        
        vs = JDFileManager::Instance()->ReadFile(vertShader);
        fs = JDFileManager::Instance()->ReadFile(fragShader);
        
        const char * ff = fs;
        const char * vv = vs;
        
        
        glShaderSource(this->vertexShader, 1, &vv, NULL);
        glShaderSource(this->fragmentShader, 1, &ff, NULL);
        
        free(vs);
        free(fs);
        
        glCompileShader(this->vertexShader);
        glCompileShader(this->fragmentShader);
        printf("Vertex\n");
        printShaderInfoLog(this->vertexShader);
        printf("Fragment\n");
        printShaderInfoLog(this->fragmentShader);
        
        this->shaderProgram = glCreateProgram();
        
        glAttachShader(this->shaderProgram,this->fragmentShader);
        glAttachShader(this->shaderProgram,this->vertexShader);
        
        //glProgramParameteriEXT(this->Program,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
        //glProgramParameteriEXT(this->Program,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
        
        //int temp;
        //glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
        ///glProgramParameteriEXT(this->Program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
        
        glLinkProgram(this->shaderProgram);
        //glUseProgram(this->Program);
        printProgramInfoLog(this->shaderProgram);
        
        glDeleteShader(this->vertexShader);
        glDeleteShader(this->fragmentShader);
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

