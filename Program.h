//
//  Program.h
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

#include <stdio.h>
//#include <AL/alut.h>
#include <stdlib.h>
#include "ImageLibManager.h"
#include <vector>
#include "Entity.h"
#include "Level.h"
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "Camera.h"


#include "PhysxManager.h"
#include "Model.h"
#include "Shader.h"
#include "PointLight.h"

class Program
{
public:
    
    int ScreenWidth;
    int ScreenHeight;
    Level* TheLevel;
    glm::vec3 hu;
    
    Program();
    ~Program();
    
    bool Init(int argc, char** argv);
    void render();
    void mainLoop();
    
    static void reshape(GLFWwindow* window, int width, int height);
    
    static void keyb(unsigned char key, int x, int y);
    
    void SetLevel(Level* level);
    
    static float testx;
    static float testy;
    static float testz;
    
    GLFWwindow* window;
    
    Camera* MainCamera;
    Shader* shader;
    Shader* shaderGeometryPass;
    Shader* shaderLightingPass;
    Shader* stencilPassShader;
    
    // Load models
    Model* Nanosuit;
    Model* SphereModel;
    GLint ModeLoc;
    bool ModelLocSet;
    
   
    
    std::vector<PointLight> PointLights;
    
    glm::mat4 projection;
    glm::mat4 view;
    
    GLuint gBuffer;
    GLuint gPosition, gNormal, gAlbedoSpec;
    GLuint FinalTexture;
    GLuint rboDepth;
    
    
private:
    GLuint quadVAO = 0;
    GLuint quadVBO;
    void RenderQuad();
    
    double CurrentTime;
    int NBFrames;
    double LastTime;
    
    void GeoPass();
    void StencilPass();
    void LightPass();
    
    void CalcMS();
    
    
};
#endif