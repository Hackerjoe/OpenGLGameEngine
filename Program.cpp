//
//  Program.cpp
//  OpenGL
//
//  Created by Joseph Despain on 5/23/15.
//  Copyright (c) 2015 Joseph Despain. All rights reserved.
//



#include "Program.h"
#include <iostream>
#include "HIDManager.h"





Program::Program()
{
    ScreenWidth = 960;
    ScreenHeight = 540;
    MainCamera = new Camera(ScreenWidth,ScreenHeight);
    
    
}

float Program::testx = 1;
float Program::testy = 0;
float Program::testz = 5;



bool Program::Init(int argc, char** argv)
{
    
    if (!glfwInit())
        return false;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    

    //glEnable(GL_TEXTURE_2D);
    glewExperimental = GL_TRUE;
    glewInit();
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    
    //Init devil for image reading
    ImageLibManager::Instance()->Init();
    PhysxManager::Instance()->Init();

    glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);
    
    
    glViewport(0, 0, ScreenWidth, ScreenHeight);
    cout << "Framebuffer size" << ScreenWidth << ScreenHeight << endl;
    
    
    TheLevel = new Level(ScreenWidth,ScreenHeight);
    shaderGeometryPass = new Shader("gbuffer.vert", "gbuffer.frag");
    shaderLightingPass = new Shader("deferredlighting.vert", "deferredlighting.frag");
    shader = new Shader("simple.vert","simple.frag");
    
    shaderLightingPass->UseShader();
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderLightingPass->GetShader(), "gAlbedoSpec"), 2);
    
    // Load models

    Nanosuit = new Model("Nanosuit/nanosuit.obj");
    SphereModel = new Model("sphere.obj");

    // Set up G-Buffer
    // 3 textures:
    // 1. Positions (RGB)
    // 2. Color (RGB) + Specular (A)
    // 3. Normals (RGB)
  
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    
    // - Position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // - Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // - Color + Specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    // Final texture that is drawn to the screen
    glGenTextures(1, &FinalTexture);
    glBindTexture(GL_TEXTURE_2D, FinalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, FinalTexture, 0);
    // - Create and attach depth texure
    glGenTextures(1, &rboDepth);
    glBindTexture(GL_TEXTURE_2D, rboDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, ScreenWidth, ScreenHeight, 0, GL_DEPTH_STENCIL,GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rboDepth, 0);
    
    
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    projection = glm::perspective(45.0f, (float)ScreenWidth/(float)ScreenHeight, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0, 1, 4), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    mainLoop();
    
        
    return true;
}

void Program::mainLoop()
{
    NBFrames = 0;
    LastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        render();
    }
}

void Program::render()
{
    testx += 0.0001;
    
    projection = glm::perspective(45.0f, (float)ScreenWidth/(float)ScreenHeight, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(testx, 0, 1), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawBuffer();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    
    glEnable(GL_STENCIL_TEST);
    
    GeoPass();

    StencilPass();

    LightPass();
    
    glDisable(GL_STENCIL_TEST);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT3);
    
    glBlitFramebuffer(0, 0, ScreenWidth, ScreenHeight,0, 0, ScreenWidth, ScreenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
    glfwSwapInterval(0);
    glfwSwapBuffers(window);
    
    glfwPollEvents();
    
    //Calc how long to calculate frame
    CalcMS();
    

    
}

void Program::GeoPass()
{
    //Bind gBuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    //
    glStencilFunc(GL_ALWAYS, 0, 0x00);
    
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glDepthMask(GL_TRUE);
    
    // Clear Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    //Use ShaderGeo shader to draw into gBuffer
    shaderGeometryPass->UseShader();
    
    // Create Model 4x4Matix for nanosuit model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, -1.75, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    //Draw into model into gBuffer
    Nanosuit->Draw(*shaderGeometryPass);
    model = glm::translate(model, glm::vec3(0.0f, 0, -10)); // Translate it down a bit so it's at the center of the scene

    glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    Nanosuit->Draw(*shaderGeometryPass);
    glDepthMask(GL_FALSE);
    
}

void Program::StencilPass()
{
    
    glDrawBuffer(GL_NONE);

    glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_CULL_FACE);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    
    // We need the stencil test to be enabled but we want it
    // to0 succeed always. Only the depth test matters.
    glStencilFunc(GL_ALWAYS, 0, 0x00);
    
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    
    
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 0, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
    
    shader->UseShader();
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));

    SphereModel->Draw(*shader);
    
}

void Program::LightPass()
{
    
    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    //glStencilMask(0);
    glStencilFunc(GL_NOTEQUAL, 0,0xff);
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 0, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
    
    shaderLightingPass->UseShader();
    
    // Insert values into shader progam
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderLightingPass->GetShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    SphereModel->Draw(*shaderLightingPass);
    
    glCullFace(GL_BACK);
    
    glDisable(GL_BLEND);
}



void Program::CalcMS()
{
    CurrentTime = glfwGetTime();
    NBFrames++;
    if ( CurrentTime - LastTime >= 1.0 ){
        printf("%f ms/frame\n", 1000/double(NBFrames));
        NBFrames = 0;
        LastTime += 1.0;
    }
}

void Program::reshape(GLFWwindow* window, int width, int height)
{
    
}

void Program::SetLevel(Level* level)
{
    TheLevel = level;
}

Program::~Program()
{
    glfwTerminate();
}
