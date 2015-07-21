//
//  TestInstancingComp.cpp
//  OpenGL
//
//  Created by Joseph Despain on 7/20/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#include "TestInstancingComp.h"

TestInstComp::TestInstComp():Component()
{
    Vector3 Positions[100];
    int index = 0;
    GLfloat offset = 1.0;
    for (GLint y = -10; y < 10; y+=2)
    {
        for (GLint x = -10; x < 10; x +=2)
        {
            Vector3 pos;
            pos.x = (GLfloat)x / 10.0f + offset;
            pos.y = (GLfloat)y / 10.0f + offset;
            Positions[index++] = pos;
        }
    }
    setShaders("InstanceVert.vert", "InstanceFrag.frag");
    glUseProgram(this->Program);
    
    for (GLuint i = 0; i < 100; i++) {
        std::stringstream ss;
        std::string index;
        ss << i;
        index = ss.str();
        GLint loc = glGetUniformLocation(this->Program,("offsets[" + index + "]").c_str());
        glUniform2f(loc,Positions[i].x,Positions[i].y);
        
        
    }
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
    
    // generate a new VBO and get the associated ID
    glGenBuffersARB(1, &vboId);
    
    // bind VBO in order to use
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
    
    // upload data to VBO
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STATIC_DRAW_ARB);
}

void TestInstComp::Update()
{
    glPushMatrix();
    //glEnable(GL_TEXTURE_2D);
    //glLinkProgram(this->Program);
    glUseProgram(this->Program);
    //glTranslatef(Parent->Postion->x, Parent->Postion->y, Parent->Postion->z);
    //glRotatef(t, 1, 0, 0);
    //glMultMatrixf(Parent->Matrix);
    // glRotatef(20, 1, 0,0);
    //float m[16];
    //glGetFloatv (GL_MODELVIEW_MATRIX, m);
    
    //glutSolidOctahedron();
    //glDisable(GL_TEXTURE_2D);
    
    
    
    
    // it is safe to delete after copying data to VBO
    
    
    //glScalef(.5, .5, .5);
    
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
    
    // enable vertex arrays
    //glEnableClientState(GL_NORMAL_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    
    // before draw, specify vertex and index arrays with their offsets
    //glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
    //glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices)+sizeof(normals)));
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3*12);
    //glDrawArraysInstanced(GL_TRIANGLES, 0, 3*12, 100);
    //glDrawElements(GL_TRIANGLES, 12*3, GL_FLOAT,0 );
    
    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    //glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    
    // it is good idea to release VBOs with ID 0 after use.
    // Once bound with 0, all pointers in gl*Pointer() behave as real
    // pointer, so, normal vertex array operations are re-activated
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    
    
    
    //glScalef(.5, .5, .5);
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_FLOAT, 0, g_vertex_buffer_data);
    //glDrawArrays(GL_TRIANGLES, 0, 3*12);
    //glDisable(GL_VERTEX_ARRAY);
    //glutWireTeapot(1);
    
    
    //glDisable(GL_TEXTURE_2D);
    //glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    
}

void TestInstComp::setShaders(std::string vert,std::string frag)
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
        
        glProgramParameteriARB(this->Program,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
        glProgramParameteriARB(this->Program,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);
        
        
        int temp;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
        glProgramParameteriEXT(this->Program,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
        
        glLinkProgram(this->Program);
        glUseProgram(this->Program);
        printf("VertexComp \n");
        printShaderInfoLog(this->Vertex);
        printf("FragComp \n");
        printShaderInfoLog(this->Fragment);
        printProgramInfoLog(this->Program);
    }else{
        printf("Shaders are NULL");
    }
    
}

void TestInstComp::printShaderInfoLog(GLuint obj)
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

void TestInstComp::printProgramInfoLog(GLuint obj)
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